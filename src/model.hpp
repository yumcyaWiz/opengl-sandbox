#pragma once

#include <assimp/scene.h>

#include <optional>
#include <string>
#include <vector>

#include "mesh.hpp"
#include "shader.hpp"

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
  void load_model(const std::filesystem::path& filepath);

  // draw model by given shader
  void draw(const Shader& shader) const;

  // destroy model object
  void destroy();

 private:
  std::vector<Mesh> meshes;
  std::vector<Texture> textures;

  // parse assimp node object
  void process_node(const aiNode* node, const aiScene* scene,
                    const std::filesystem::path& parentPath);

  // parse assimp mesh object
  Mesh process_mesh(const aiMesh* mesh, const aiScene* scene,
                    const std::filesystem::path& parentPath);

  std::optional<std::size_t> loadTexture(
      const aiMaterial* material, const TextureType& type,
      const std::filesystem::path& parentPath);

  std::optional<std::size_t> hasTexture(
      const std::filesystem::path& filepath) const;
};

}  // namespace ogls