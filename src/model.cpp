#include "ogls/model.hpp"

#include <filesystem>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "spdlog/spdlog.h"

namespace ogls {

Model::Model() {}

Model::Model(const std::filesystem::path& filepath) { loadModel(filepath); }

Model::operator bool() const { return meshes.size() > 0; }

void Model::loadModel(const std::filesystem::path& filepath) {
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
  processNode(scene->mRootNode, scene, ps.parent_path());

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

void Model::draw(const Shader& shader) const {
  // draw all meshes
  for (std::size_t i = 0; i < meshes.size(); i++) {
    meshes[i].draw(shader, textures);
  }
}

void Model::destroy() {
  // destroy all meshes
  for (auto& mesh : meshes) {
    mesh.destroy();
  }
  meshes.clear();

  // destroy all textures
  for (auto& texture : textures) {
    texture.destroy();
  }
  textures.clear();
}

void Model::processNode(const aiNode* node, const aiScene* scene,
                        const std::filesystem::path& parentPath) {
  // process all the node's meshes
  for (std::size_t i = 0; i < node->mNumMeshes; ++i) {
    const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene, parentPath));
  }

  for (std::size_t i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, parentPath);
  }
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene,
                        const std::filesystem::path& parentPath) {
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
    material.diffuseMap = loadTexture(mat, TextureType::Diffuse, parentPath);

    // specular map
    material.specularMap = loadTexture(mat, TextureType::Specular, parentPath);

    // ambient map
    material.ambientMap = loadTexture(mat, TextureType::Ambient, parentPath);

    // emissive map
    material.emissiveMap = loadTexture(mat, TextureType::Emissive, parentPath);

    // height map
    material.heightMap = loadTexture(mat, TextureType::Height, parentPath);

    // normal map
    material.normalMap = loadTexture(mat, TextureType::Normal, parentPath);

    // shininess map
    material.shininessMap =
        loadTexture(mat, TextureType::Shininess, parentPath);

    // displacement map
    material.displacementMap =
        loadTexture(mat, TextureType::Displacement, parentPath);

    // light map
    material.lightMap = loadTexture(mat, TextureType::Light, parentPath);
  }

  return Mesh(vertices, indices, material);
}

std::optional<std::size_t> Model::loadTexture(
    const aiMaterial* material, const TextureType& type,
    const std::filesystem::path& parentPath) {
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
  if (material->GetTextureCount(aiTexType) == 0) {
    return std::nullopt;
  }

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
    const std::filesystem::path& filepath) const {
  for (std::size_t i = 0; i < textures.size(); ++i) {
    const Texture& texture = textures[i];
    if (texture.filepath == filepath) {
      return i;
    }
  }
  return std::nullopt;
}

}  // namespace ogls