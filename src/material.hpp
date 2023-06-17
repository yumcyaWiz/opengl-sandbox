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

}  // namespace ogls