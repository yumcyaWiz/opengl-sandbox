#include "ogls/model.hpp"

#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>
#include <filesystem>
#include <iostream>

namespace ogls {

Model::Model() {}

Model::Model(const std::string& filepath) { loadModel(filepath); }

Model::operator bool() const { return meshes.size() > 0; }

void Model::loadModel(const std::string& filepath) {
  // load model with assimp
  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs |
                                      aiProcess_GenNormals);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "[Assimp] " << importer.GetErrorString() << std::endl;
    return;
  }

  // process scene graph
  const std::filesystem::path ps(filepath);
  processNode(scene->mRootNode, scene, ps.parent_path());

  // show info
  std::cout << "[Model] " << filepath << " loaded." << std::endl;
  std::cout << "[Model] number of meshes: " << meshes.size() << std::endl;

  std::size_t nVertices = 0;
  std::size_t nFaces = 0;
  for (std::size_t i = 0; i < meshes.size(); ++i) {
    nVertices += meshes[i].vertices.size();
    nFaces += meshes[i].indices.size() / 3;
  }
  std::cout << "[Model] number of vertices: " << nVertices << std::endl;
  std::cout << "[Model] number of faces: " << nFaces << std::endl;
  std::cout << "[Model] number of textures: " << textures.size() << std::endl;
}

void Model::draw(const Shader& shader) const {
  for (std::size_t i = 0; i < meshes.size(); i++) {
    meshes[i].draw(shader, textures);
  }
}

void Model::destroy() {
  for (auto& mesh : meshes) {
    mesh.destroy();
  }
  meshes.clear();

  for (auto& texture : textures) {
    texture.destroy();
  }
  textures.clear();
}

void Model::processNode(const aiNode* node, const aiScene* scene,
                        const std::string& parentPathStr) {
  // process all the node's meshes
  for (std::size_t i = 0; i < node->mNumMeshes; ++i) {
    const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene, parentPathStr));
  }

  for (std::size_t i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, parentPathStr);
  }
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene,
                        const std::string& parentPathStr) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;

  // vertices
  for (std::size_t i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;
    vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                mesh->mVertices[i].z);

    if (mesh->mNormals) {
      vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                mesh->mNormals[i].z);
    } else {
      vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (mesh->mTextureCoords[0]) {
      vertex.texcoords =
          glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    } else {
      vertex.texcoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

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

    // shininess
    mat->Get(AI_MATKEY_SHININESS, material.shininess);

    // diffuse textures
    material.diffuseMap = loadTexture(mat, TextureType::Diffuse, parentPathStr);

    // specular textures
    material.specularMap =
        loadTexture(mat, TextureType::Specular, parentPathStr);
  }

  return Mesh(vertices, indices, material);
}

std::optional<std::size_t> Model::loadTexture(
    const aiMaterial* material, const TextureType& type,
    const std::string& parentPathStr) {
  aiTextureType aiTexType;
  switch (type) {
    case TextureType::Diffuse:
      aiTexType = aiTextureType_DIFFUSE;
      break;
    case TextureType::Specular:
      aiTexType = aiTextureType_SPECULAR;
      break;
  }

  if (material->GetTextureCount(aiTexType) == 0) {
    return std::nullopt;
  }

  // get texture filepath
  aiString str;
  material->GetTexture(aiTexType, 0, &str);
  const std::filesystem::path parentPath(parentPathStr);
  const std::filesystem::path texturePath(str.C_Str());
  const std::string texturePathStr = (parentPath / texturePath).native();

  // load texture if we don't have it
  const auto index = hasTexture(texturePathStr);
  if (!index) {
    textures.emplace_back(texturePathStr, TextureType::Diffuse);
    return textures.size() - 1;
  } else {
    return index.value();
  }
}

std::optional<std::size_t> Model::hasTexture(
    const std::string& filepath) const {
  for (std::size_t i = 0; i < textures.size(); ++i) {
    const Texture& texture = textures[i];
    if (texture.filepath == filepath) {
      return i;
    }
  }
  return std::nullopt;
}

}  // namespace ogls