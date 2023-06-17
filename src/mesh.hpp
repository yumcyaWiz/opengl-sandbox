#pragma once

#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
//
#include "buffer.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex-array-object.hpp"

namespace ogls
{

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
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;

  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<unsigned int>& indices, const Material& material);

  void release();

  // draw mesh by given shader
  void draw(const Pipeline& pipeline, const Shader& shader,
            const std::vector<Texture>& textures) const;

 private:
  VertexArrayObject vao;
  Buffer vbo;
  Buffer ebo;
};

}  // namespace ogls