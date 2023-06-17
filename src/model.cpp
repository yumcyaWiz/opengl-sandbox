#include "model.hpp"

#include <filesystem>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "spdlog/spdlog.h"

namespace ogls
{

Model::Model() {}

Model::Model(const std::filesystem::path& filepath) { load_model(filepath); }

Model::operator bool() const { return meshes.size() > 0; }

void Model::load_model(const std::filesystem::path& filepath)
{
  // load model with assimp
  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(filepath.generic_string().c_str(),
                        aiProcess_Triangulate | aiProcess_FlipUVs |
                            aiProcess_GenNormals | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    spdlog::error(importer.GetErrorString());
    return;
  }

  // process scene graph
  const std::filesystem::path ps(filepath);
  process_node(scene->mRootNode, scene, ps.parent_path());

  // show info
  spdlog::info("[Model] " + filepath.string() + " loaded.");
  spdlog::info("[Model] number of meshes: " + std::to_string(meshes.size()));

  std::size_t nVertices = 0;
  std::size_t nFaces = 0;
  for (std::size_t i = 0; i < meshes.size(); ++i) {
    nVertices += meshes[i].vertices.size();
    nFaces += meshes[i].indices.size() / 3;
  }
  spdlog::info("[Model] number of vertices: " + std::to_string(nVertices));
  spdlog::info("[Model] number of faces: " + std::to_string(nFaces));
  spdlog::info("[Model] number of textures: " +
               std::to_string(textures.size()));
}

void Model::draw(const Shader& shader) const
{
  // draw all meshes
  for (std::size_t i = 0; i < meshes.size(); i++) {
    meshes[i].draw(shader, textures);
  }
}

void Model::destroy()
{
  // destroy all meshes
  for (auto& mesh : meshes) { mesh.destroy(); }
  meshes.clear();

  // destroy all textures
  for (auto& texture : textures) { texture.destroy(); }
  textures.clear();
}

void Model::process_node(const aiNode* node, const aiScene* scene,
                         const std::filesystem::path& parentPath)
{
  // process all the node's meshes
  for (std::size_t i = 0; i < node->mNumMeshes; ++i) {
    const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(process_mesh(mesh, scene, parentPath));
  }

  for (std::size_t i = 0; i < node->mNumChildren; i++) {
    process_node(node->mChildren[i], scene, parentPath);
  }
}

Mesh Model::process_mesh(const aiMesh* mesh, const aiScene* scene,
                         const std::filesystem::path& parentPath)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;

  spdlog::info("[Mesh] Processing " + std::string(mesh->mName.C_Str()));
  spdlog::info("[Mesh] number of vertices " +
               std::to_string(mesh->mNumVertices));

  // vertices
  for (std::size_t i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;
    // position
    vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                mesh->mVertices[i].z);

    // normal
    if (mesh->mNormals) {
      vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                mesh->mNormals[i].z);
    } else {
      vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    // texcoords
    if (mesh->mTextureCoords[0]) {
      vertex.texcoords =
          glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    } else {
      vertex.texcoords = glm::vec2(0.0f, 0.0f);
    }

    // tangent
    if (mesh->mTangents) {
      vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y,
                                 mesh->mTangents[i].z);
    }

    vertices.push_back(vertex);
  }

  spdlog::info("[Mesh] number of faces " + std::to_string(mesh->mNumFaces));

  // indices
  for (std::size_t i = 0; i < mesh->mNumFaces; ++i) {
    const aiFace& face = mesh->mFaces[i];
    for (std::size_t j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // compute dn/dp, dn/dv
  for (std::size_t i = 0; i < indices.size(); i += 3) {
    const unsigned int idx1 = indices[i];
    const unsigned int idx2 = indices[i + 1];
    const unsigned int idx3 = indices[i + 2];

    const glm::vec3 dn1 = vertices[idx2].normal - vertices[idx1].normal;
    const glm::vec3 dn2 = vertices[idx3].normal - vertices[idx1].normal;
    const float du1 = vertices[idx2].texcoords.x - vertices[idx1].texcoords.x;
    const float du2 = vertices[idx3].texcoords.x - vertices[idx1].texcoords.x;
    const float dv1 = vertices[idx2].texcoords.y - vertices[idx1].texcoords.y;
    const float dv2 = vertices[idx3].texcoords.y - vertices[idx1].texcoords.y;

    const float invDeterminant = 1.0f / (du1 * dv2 - dv1 * du2);

    const glm::vec3 dndu = invDeterminant * (dv2 * dn1 - dv1 * dn2);
    const glm::vec3 dndv = invDeterminant * (-du2 * dn1 + du1 * dn2);

    // NOTE: smoothing will give more nice result
    vertices[idx1].dndu = dndu;
    vertices[idx2].dndu = dndu;
    vertices[idx3].dndu = dndu;
    vertices[idx1].dndv = dndv;
    vertices[idx2].dndv = dndv;
    vertices[idx3].dndv = dndv;
  }

  // materials
  if (scene->mMaterials[mesh->mMaterialIndex]) {
    const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      spdlog::info("[Mesh] Diffuse Map Detected");
    }
    if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
      spdlog::info("[Mesh] Specular Map Detected");
    }
    if (mat->GetTextureCount(aiTextureType_AMBIENT) > 0) {
      spdlog::info("[Mesh] Ambient Map Detected");
    }
    if (mat->GetTextureCount(aiTextureType_EMISSIVE) > 0) {
      spdlog::info("[Mesh] Emissive Map Detected");
    }
    if (mat->GetTextureCount(aiTextureType_EMISSIVE) > 0) {
      spdlog::info("[Mesh] Emissive Map Detected");
    }
    if (mat->GetTextureCount(aiTextureType_HEIGHT) > 0) {
      spdlog::info("[Mesh] Height Map Detected");
    }
    if (mat->GetTextureCount(aiTextureType_NORMALS) > 0) {
      spdlog::info("[Mesh] Normal Map Detected");
    }
    if (mat->GetTextureCount(aiTextureType_SHININESS) > 0) {
      spdlog::info("[Mesh] Shininess Map Detected");
    }
    if (mat->GetTextureCount(aiTextureType_DISPLACEMENT) > 0) {
      spdlog::info("[Mesh] Displacement Map Detected");
    }
    if (mat->GetTextureCount(aiTextureType_LIGHTMAP) > 0) {
      spdlog::info("[Mesh] Light Map Detected");
    }

    // kd
    aiColor3D color;
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    // convert sRGB to linear
    material.kd =
        glm::pow(glm::vec3(color.r, color.g, color.b), glm::vec3(2.2f));

    // ks
    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.ks = glm::vec3(color.r, color.g, color.b);

    // ka
    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.ka = glm::vec3(color.r, color.g, color.b);

    // ke
    mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
    material.ke = glm::vec3(color.r, color.g, color.b);

    // shininess
    mat->Get(AI_MATKEY_SHININESS, material.shininess);

    // diffuse map
    material.diffuse_map = loadTexture(mat, TextureType::Diffuse, parentPath);

    // specular map
    material.specular_map = loadTexture(mat, TextureType::Specular, parentPath);

    // ambient map
    material.ambient_map = loadTexture(mat, TextureType::Ambient, parentPath);

    // emissive map
    material.emissive_map = loadTexture(mat, TextureType::Emissive, parentPath);

    // height map
    material.height_map = loadTexture(mat, TextureType::Height, parentPath);

    // normal map
    material.normal_map = loadTexture(mat, TextureType::Normal, parentPath);

    // shininess map
    material.shininess_map =
        loadTexture(mat, TextureType::Shininess, parentPath);

    // displacement map
    material.displacement_map =
        loadTexture(mat, TextureType::Displacement, parentPath);

    // light map
    material.light_map = loadTexture(mat, TextureType::Light, parentPath);
  }

  return Mesh(vertices, indices, material);
}

std::optional<std::size_t> Model::loadTexture(
    const aiMaterial* material, const TextureType& type,
    const std::filesystem::path& parentPath)
{
  aiTextureType aiTexType;
  switch (type) {
    case TextureType::Diffuse:
      aiTexType = aiTextureType_DIFFUSE;
      break;
    case TextureType::Specular:
      aiTexType = aiTextureType_SPECULAR;
      break;
    case TextureType::Ambient:
      aiTexType = aiTextureType_AMBIENT;
      break;
    case TextureType::Emissive:
      aiTexType = aiTextureType_EMISSIVE;
      break;
    case TextureType::Height:
      aiTexType = aiTextureType_HEIGHT;
      break;
    case TextureType::Normal:
      aiTexType = aiTextureType_NORMALS;
      break;
    case TextureType::Shininess:
      aiTexType = aiTextureType_SHININESS;
      break;
    case TextureType::Displacement:
      aiTexType = aiTextureType_DISPLACEMENT;
      break;
    case TextureType::Light:
      aiTexType = aiTextureType_LIGHTMAP;
      break;
  }

  // if there is no texture
  if (material->GetTextureCount(aiTexType) == 0) { return std::nullopt; }

  // get texture filepath
  aiString str;
  material->GetTexture(aiTexType, 0, &str);
  const std::filesystem::path texturePath = (parentPath / str.C_Str());

  // load texture if we don't have it
  const auto index = hasTexture(texturePath);
  if (!index) {
    textures.emplace_back(texturePath, type);
    return textures.size() - 1;
  } else {
    return index.value();
  }
}

std::optional<std::size_t> Model::hasTexture(
    const std::filesystem::path& filepath) const
{
  for (std::size_t i = 0; i < textures.size(); ++i) {
    const Texture& texture = textures[i];
    if (texture.filepath == filepath) { return i; }
  }
  return std::nullopt;
}

}  // namespace ogls