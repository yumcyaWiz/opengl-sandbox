#pragma once

#include <optional>

#include "glm/glm.hpp"

namespace ogls
{

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

}  // namespace ogls