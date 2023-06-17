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

  // does model have any meshes?
  operator bool() const;

  // load model with assimp
  void loadModel(const std::filesystem::path& filepath);

  void draw(const Pipeline& pipeline, const Shader& shader) const;

  void release();

 private:
  std::vector<Mesh> meshes;
  std::vector<Texture> textures;

  std::vector<std::filesystem::path> loaded_textures;

  // parse assimp node object
  void processAssimpNode(const aiNode* node, const aiScene* scene,
                         const std::filesystem::path& parentPath);

  // parse assimp mesh object
  Mesh processAssimpMesh(const aiMesh* mesh, const aiScene* scene,
                         const std::filesystem::path& parentPath);

  std::optional<std::size_t> loadTexture(
      const aiMaterial* material, const TextureType& type,
      const std::filesystem::path& parentPath);

  std::optional<std::size_t> getTextureIndex(
      const std::filesystem::path& filepath) const;

  static const std::map<TextureType, aiTextureType> assimp_texture_mapping;
};

}  // namespace ogls