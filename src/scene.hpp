#pragma once
#include <optional>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
//
#include "light.hpp"
#include "model.hpp"
#include "shader.hpp"

namespace ogls
{

class Scene
{
 public:
  Model model;

  std::vector<PointLight> pointLights;
  std::optional<std::size_t> pointLightIndex;
  DirectionalLight directionalLight;

  Scene();

  // draw scene
  void draw(const Shader& shader) const;

  // destroy scene object
  void destroy();

  void set_model(const Model& model);

  void add_point_light(const PointLight& light);

  void set_point_light_index(std::size_t index);

  void set_directional_light(const DirectionalLight& light);
};

}  // namespace ogls