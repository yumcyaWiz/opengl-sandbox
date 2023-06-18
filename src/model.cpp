#include "model.hpp"

#include <filesystem>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/postprocess.h"
#include "spdlog/spdlog.h"
#include "texture.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

namespace ogls
{

const std::map<TextureType, aiTextureType> Model::assimp_texture_mapping = {
    {TextureType::Diffuse, aiTextureType_DIFFUSE},
    {TextureType::Specular, aiTextureType_SPECULAR},
    {TextureType::Ambient, aiTextureType_AMBIENT},
    {TextureType::Emissive, aiTextureType_EMISSIVE},
    {TextureType::Height, aiTextureType_HEIGHT},
    {TextureType::Normal, aiTextureType_NORMALS},
    {TextureType::Shininess, aiTextureType_SHININESS},
    {TextureType::Displacement, aiTextureType_DISPLACEMENT},
    {TextureType::Light, aiTextureType_LIGHTMAP}};

GLuint Model::getTextureInternalFormat(const TextureType& type)
{
  switch (type) {
    case TextureType::Diffuse:
    case TextureType::Ambient:
    case TextureType::Emissive:
      return GL_SRGB;
    default:
      return GL_RGB;
  }
}

Model::Model() {}

Model::Model(const std::filesystem::path& filepath) { loadModel(filepath); }

Model::Model(Model&& other)
    : meshes(std::move(other.meshes)),
      textures(std::move(other.textures)),
      loaded_textures(std::move(other.loaded_textures))
{
}

Model& Model::operator=(Model&& other)
{
  if (this == &other) return *this;
  meshes = std::move(other.meshes);
  textures = std::move(other.textures);
  loaded_textures = std::move(other.loaded_textures);
  return *this;
}

Model::operator bool() const { return meshes.size() > 0; }

uint32_t Model::getNumberOfVertices() const
{
  uint32_t n_vertices = 0;
  for (std::size_t i = 0; i < meshes.size(); ++i) {
    n_vertices += meshes[i].getNumberOfVertices();
  }
  return n_vertices;
}

uint32_t Model::getNumberOfFaces() const
{
  uint32_t n_faces = 0;
  for (std::size_t i = 0; i < meshes.size(); ++i) {
    n_faces += meshes[i].getNumberOfFaces();
  }
  return n_faces;
}

uint32_t Model::getNumberOfTextures() const { return textures.size(); }

void Model::loadModel(const std::filesystem::path& filepath)
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
  processAssimpNode(scene->mRootNode, scene, ps.parent_path());

  // show info
  spdlog::debug("[Model] " + filepath.string() + " loaded.");
  spdlog::debug("[Model] number of meshes: " + std::to_string(meshes.size()));

  spdlog::debug("[Model] number of vertices: " +
                std::to_string(getNumberOfVertices()));
  spdlog::debug("[Model] number of faces: " +
                std::to_string(getNumberOfFaces()));
  spdlog::debug("[Model] number of textures: " +
                std::to_string(getNumberOfTextures()));
}

void Model::draw(const Pipeline& pipeline) const
{
  // draw all meshes
  for (std::size_t i = 0; i < meshes.size(); i++) {
    meshes[i].draw(pipeline, textures);
  }
}

void Model::processAssimpNode(const aiNode* node, const aiScene* scene,
                              const std::filesystem::path& parentPath)
{
  // process all the node's meshes
  for (std::size_t i = 0; i < node->mNumMeshes; ++i) {
    const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processAssimpMesh(mesh, scene, parentPath));
  }

  for (std::size_t i = 0; i < node->mNumChildren; i++) {
    processAssimpNode(node->mChildren[i], scene, parentPath);
  }
}

std::vector<Vertex> Model::getVerticesFromAssimpMesh(const aiMesh* mesh)
{
  std::vector<Vertex> ret;

  for (std::size_t i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;
    // position
    vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                mesh->mVertices[i].z);

    // normal
    if (mesh->mNormals) {
      vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                mesh->mNormals[i].z);
    }

    // texture coordinates
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

    ret.push_back(vertex);
  }

  return ret;
}

std::vector<uint32_t> Model::getIndicesFromAssimpMesh(const aiMesh* mesh)
{
  std::vector<uint32_t> ret;

  for (std::size_t i = 0; i < mesh->mNumFaces; ++i) {
    const aiFace& face = mesh->mFaces[i];
    for (std::size_t j = 0; j < face.mNumIndices; ++j) {
      ret.push_back(face.mIndices[j]);
    }
  }

  return ret;
}

Material Model::getMaterialFromAssimpMesh(
    const aiMesh* mesh, const aiScene* scene,
    const std::filesystem::path& parentPath)
{
  Material ret;

  if (!scene->mMaterials[mesh->mMaterialIndex]) return ret;
  const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

  // kd
  aiColor3D color;
  mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
  // convert sRGB to linear
  ret.kd = glm::pow(glm::vec3(color.r, color.g, color.b), glm::vec3(2.2f));

  // ks
  mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
  ret.ks = glm::vec3(color.r, color.g, color.b);

  // ka
  mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
  ret.ka = glm::vec3(color.r, color.g, color.b);

  // ke
  mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
  ret.ke = glm::vec3(color.r, color.g, color.b);

  // shininess
  mat->Get(AI_MATKEY_SHININESS, ret.shininess);

  // diffuse map
  ret.diffuse_map = loadTexture(mat, TextureType::Diffuse, parentPath);

  // specular map
  ret.specular_map = loadTexture(mat, TextureType::Specular, parentPath);

  // ambient map
  ret.ambient_map = loadTexture(mat, TextureType::Ambient, parentPath);

  // emissive map
  ret.emissive_map = loadTexture(mat, TextureType::Emissive, parentPath);

  // height map
  ret.height_map = loadTexture(mat, TextureType::Height, parentPath);

  // normal map
  ret.normal_map = loadTexture(mat, TextureType::Normal, parentPath);

  // shininess map
  ret.shininess_map = loadTexture(mat, TextureType::Shininess, parentPath);

  // displacement map
  ret.displacement_map =
      loadTexture(mat, TextureType::Displacement, parentPath);

  // light map
  ret.light_map = loadTexture(mat, TextureType::Light, parentPath);

  return ret;
}

Mesh Model::processAssimpMesh(const aiMesh* mesh, const aiScene* scene,
                              const std::filesystem::path& parentPath)
{
  spdlog::debug("[Mesh] Processing " + std::string(mesh->mName.C_Str()));
  spdlog::debug("[Mesh] number of vertices " +
                std::to_string(mesh->mNumVertices));
  spdlog::debug("[Mesh] number of faces " + std::to_string(mesh->mNumFaces));

  std::vector<Vertex> vertices = getVerticesFromAssimpMesh(mesh);
  const std::vector<uint32_t> indices = getIndicesFromAssimpMesh(mesh);

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

    // TODO: smoothing will give more nice result
    vertices[idx1].dndu = dndu;
    vertices[idx2].dndu = dndu;
    vertices[idx3].dndu = dndu;
    vertices[idx1].dndv = dndv;
    vertices[idx2].dndv = dndv;
    vertices[idx3].dndv = dndv;
  }

  const Material material = getMaterialFromAssimpMesh(mesh, scene, parentPath);

  return Mesh(vertices, indices, material);
}

std::vector<uint8_t> Model::loadImage(const std::filesystem::path& filepath,
                                      glm::vec2& resolution)
{
  int x, y, c;
  unsigned char* image = stbi_load(filepath.c_str(), &x, &y, &c, 3);

  if (!image) {
    spdlog::error("[Model] Failed to load image " + filepath.string());
    throw std::runtime_error("Failed to load image " + filepath.string());
  }

  resolution = glm::vec2(x, y);

  std::vector<uint8_t> ret(x * y * c);
  std::memcpy(ret.data(), image, ret.size());

  stbi_image_free(image);

  return ret;
}

std::optional<std::size_t> Model::loadTexture(
    const aiMaterial* material, const TextureType& type,
    const std::filesystem::path& parentPath)
{
  const aiTextureType aiTexType = assimp_texture_mapping.at(type);

  // if there is no texture
  if (material->GetTextureCount(aiTexType) == 0) { return std::nullopt; }

  // get texture filepath
  aiString str;
  material->GetTexture(aiTexType, 0, &str);
  const std::filesystem::path texturePath = (parentPath / str.C_Str());

  // load texture if it's not loaded
  const auto index = getTextureIndex(texturePath);
  if (index) { return index; }

  glm::vec2 resolution;
  const std::vector<uint8_t> image = loadImage(texturePath, resolution);
  loaded_textures.emplace_back(texturePath);

  const GLuint internal_format = getTextureInternalFormat(type);
  Texture texture(resolution, internal_format, GL_RGB, GL_UNSIGNED_BYTE);
  texture.setImage(image.data(), resolution, internal_format, GL_RGB,
                   GL_UNSIGNED_BYTE);

  textures.emplace_back(std::move(texture));

  return textures.size() - 1;
}

std::optional<std::size_t> Model::getTextureIndex(
    const std::filesystem::path& filepath) const
{
  for (std::size_t i = 0; i < loaded_textures.size(); ++i) {
    if (loaded_textures[i] == filepath) return i;
  }
  return std::nullopt;
}

}  // namespace ogls