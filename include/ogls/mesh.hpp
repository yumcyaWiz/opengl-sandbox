#ifndef _OGLS_MESH_H
#define _OGLS_MESH_H
#include <optional>
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

  Vertex() : position{0.0f}, normal{0.0f}, texcoords{0.0f} {}
};

struct Material {
  glm::vec3 kd;  // diffuse color
  glm::vec3 ks;  // specular color
  glm::vec3 ka;  // ambient color
  glm::vec3 ke;  // emissive color
  float shininess;

  std::optional<unsigned int> diffuseMap;    // index of diffuse map texture
  std::optional<unsigned int> specularMap;   // index of specular map texture
  std::optional<unsigned int> ambientMap;    // index of ambient map texture
  std::optional<unsigned int> emissiveMap;   // index of emissive map texture
  std::optional<unsigned int> heightMap;     // index of height map texture
  std::optional<unsigned int> normalMap;     // index of normal map texture
  std::optional<unsigned int> shininessMap;  // index of shininess map texture
  std::optional<unsigned int>
      displacementMap;                   // index of displacement map texture
  std::optional<unsigned int> lightMap;  // index of light map texture

  Material() : kd{0.0f}, ks{0.0f}, ka{0.0f}, ke{0.0f}, shininess{0.0f} {}
};

class Mesh {
 public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;

  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<unsigned int>& indices, const Material& material);

  // destroy mesh object
  void destroy();

  // draw mesh by given shader
  void draw(const Shader& shader, const std::vector<Texture>& textures) const;

 private:
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
};

}  // namespace ogls

#endif