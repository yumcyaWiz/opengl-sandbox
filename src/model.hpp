#pragma once

#include <assimp/scene.h>

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "assimp/material.h"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace ogls
{

class Model
{
 public:
  Model();
  Model(const std::filesystem::path& filepath);
  Model(const Model& other) = delete;
  Model(Model&& other);
  ~Model() = default;

  Model& operator=(const Model& other) = delete;
  Model& operator=(Model&& other);

  // does model have any meshes?
  operator bool() const;

  // load model with assimp
  void loadModel(const std::filesystem::path& filepath);

  uint32_t getNumberOfVertices() const;
  uint32_t getNumberOfFaces() const;
  uint32_t getNumberOfTextures() const;

  void draw(const Pipeline& pipeline) const;

 private:
  std::vector<Mesh> meshes;
  std::vector<Material> materials;
  std::vector<Texture> textures;

  using assimpMaterialIndex = uint32_t;
  std::vector<assimpMaterialIndex> loaded_materials;
  std::vector<std::filesystem::path> loaded_textures;

  void processAssimpNode(const aiNode* node, const aiScene* scene,
                         const std::filesystem::path& parentPath);

  Mesh processAssimpMesh(const aiMesh* mesh, const aiScene* scene,
                         const std::filesystem::path& parentPath);

  // return offset of the material
  std::size_t loadMaterial(const aiScene* scene, assimpMaterialIndex index,
                           const std::filesystem::path& parent_path);

  // return offset of the texture
  std::optional<std::size_t> loadTexture(
      const aiMaterial* material, const TextureType& type,
      const std::filesystem::path& parentPath);

  // return offset of the texture
  std::optional<std::size_t> getTextureIndex(
      const std::filesystem::path& filepath) const;

  // return offset of the material
  std::optional<std::size_t> getMaterialIndex(
      assimpMaterialIndex assimp_material_id) const;

  static std::vector<Vertex> getVerticesFromAssimpMesh(const aiMesh* mesh);
  static std::vector<uint32_t> getIndicesFromAssimpMesh(const aiMesh* mesh);
  Material getMaterialFromAssimpMesh(const aiMaterial* material,
                                     const std::filesystem::path& parent_path);

  static GLuint getTextureInternalFormat(const TextureType& type);

  static std::vector<uint8_t> loadImage(const std::filesystem::path& filepath,
                                        glm::vec2& resolution);

  static const std::map<TextureType, aiTextureType> assimp_texture_mapping;
};

}  // namespace ogls