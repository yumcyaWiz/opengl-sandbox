#pragma once

#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
//
#include "buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex-array-object.hpp"

namespace ogls
{

using MaterialID = uint32_t;
using TextureID = uint32_t;

struct Material {
  // diffuse color
  glm::vec3 kd = glm::vec3(0.0f);
  // specular color
  glm::vec3 ks = glm::vec3(0.0f);
  // ambient color
  glm::vec3 ka = glm::vec3(0.0f);
  // emissive color
  glm::vec3 ke = glm::vec3(0.0f);
  float shininess = 0.0f;

  // index of diffuse map texture
  std::optional<unsigned int> diffuse_map = std::nullopt;
  // index of specular map texture
  std::optional<unsigned int> specular_map = std::nullopt;
  // index of ambient map texture
  std::optional<unsigned int> ambient_map = std::nullopt;
  // index of emissive map texture
  std::optional<unsigned int> emissive_map = std::nullopt;
  // index of height map texture
  std::optional<unsigned int> height_map = std::nullopt;
  // index of normal map texture
  std::optional<unsigned int> normal_map = std::nullopt;
  // index of shininess map texture
  std::optional<unsigned int> shininess_map = std::nullopt;
  // index of displacement map texture
  std::optional<unsigned int> displacement_map = std::nullopt;
  // index of light map texture
  std::optional<unsigned int> light_map = std::nullopt;

  Material() {}
};

struct Vertex {
  glm::vec3 position;   // vertex position
  glm::vec3 normal;     // vertex normal
  glm::vec2 texcoords;  // texture coordinates
  glm::vec3 tangent;    // tangent vector(dp/du)
  glm::vec3 dndu;       // differential of normal by texcoords
  glm::vec3 dndv;       // differential of normal by texcoords

  Vertex() : position{0.0f}, normal{0.0f}, texcoords{0.0f}, tangent{0.0f} {}
};

// TODO: maybe this class should be data class and all the methods should be
// moved to Model class
class Mesh

{
 public:
  Mesh();
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<unsigned int>& indices, MaterialID material_index);
  Mesh(const Mesh& other) = delete;
  Mesh(Mesh&& other);
  ~Mesh() = default;

  Mesh& operator=(const Mesh& other) = delete;
  Mesh& operator=(Mesh&& other);

  // draw mesh by given shader
  void draw(const Pipeline& pipeline, const Material& material,
            const std::vector<Texture>& textures) const;

  uint32_t getNumberOfVertices() const;
  uint32_t getNumberOfFaces() const;
  MaterialID getMaterialID() const;

 private:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  // TODO: remove this field, this is only used in Model class
  MaterialID material_id;

  VertexArrayObject vao;
  Buffer vertex_buffer;
  Buffer index_buffer;
};

}  // namespace ogls