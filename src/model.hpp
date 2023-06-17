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

  void draw(const Pipeline& pipeline, const Shader& shader) const;

 private:
  std::vector<Mesh> meshes;
  std::vector<Texture> textures;

  std::vector<std::filesystem::path> loaded_textures;

  void processAssimpNode(const aiNode* node, const aiScene* scene,
                         const std::filesystem::path& parentPath);

  Mesh processAssimpMesh(const aiMesh* mesh, const aiScene* scene,
                         const std::filesystem::path& parentPath);

  std::optional<std::size_t> loadTexture(
      const aiMaterial* material, const TextureType& type,
      const std::filesystem::path& parentPath);

  std::optional<std::size_t> getTextureIndex(
      const std::filesystem::path& filepath) const;

  static GLuint getTextureInternalFormat(const TextureType& type);

  static const std::map<TextureType, aiTextureType> assimp_texture_mapping;
};

}  // namespace ogls