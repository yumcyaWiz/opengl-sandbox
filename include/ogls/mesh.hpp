#ifndef _OGLS_MESH_H
#define _OGLS_MESH_H
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
//
#include "shader.hpp"
#include "texture.hpp"

namespace ogls {

struct Vertex {
  glm::vec3 position;   // vertex position
  glm::vec3 normal;     // vertex normal
  glm::vec2 texcoords;  // texture coordinates
};

struct Material {
  glm::vec3 kd;  // diffuse color
  glm::vec3 ks;  // specular color
  glm::vec3 ka;  // ambient color
  float shininess;
};

class Mesh {
 public:
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<unsigned int>& indices, const Material& material,
       const std::vector<unsigned int>& indicesOfTextures);

  // destroy mesh object
  void destroy();

  // draw mesh by given shader
  void draw(const Shader& shader, const std::vector<Texture>& textures) const;

 private:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;
  std::vector<unsigned int> indicesOfTextures;  // indices of textures

  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
};

}  // namespace ogls

#endif