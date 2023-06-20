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

struct Material {
  glm::vec3 kd;  // diffuse color
  glm::vec3 ks;  // specular color
  glm::vec3 ka;  // ambient color
  glm::vec3 ke;  // emissive color
  float shininess;

  std::optional<unsigned int> diffuse_map;    // index of diffuse map texture
  std::optional<unsigned int> specular_map;   // index of specular map texture
  std::optional<unsigned int> ambient_map;    // index of ambient map texture
  std::optional<unsigned int> emissive_map;   // index of emissive map texture
  std::optional<unsigned int> height_map;     // index of height map texture
  std::optional<unsigned int> normal_map;     // index of normal map texture
  std::optional<unsigned int> shininess_map;  // index of shininess map texture
  std::optional<unsigned int>
      displacement_map;                   // index of displacement map texture
  std::optional<unsigned int> light_map;  // index of light map texture

  Material() : kd{0.0f}, ks{0.0f}, ka{0.0f}, ke{0.0f}, shininess{0.0f} {}
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

class Mesh
{
 public:
  Mesh();
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<unsigned int>& indices, const Material& material);
  Mesh(const Mesh& other) = delete;
  Mesh(Mesh&& other);
  ~Mesh() = default;

  Mesh& operator=(const Mesh& other) = delete;
  Mesh& operator=(Mesh&& other);

  // draw mesh by given shader
  void draw(const Pipeline& pipeline,
            const std::vector<Texture>& textures) const;

  uint32_t getNumberOfVertices() const;
  uint32_t getNumberOfFaces() const;

 private:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;

  VertexArrayObject vao;
  Buffer vertex_buffer;
  Buffer index_buffer;
};

}  // namespace ogls