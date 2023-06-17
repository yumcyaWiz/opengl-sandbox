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
  const Model* model = nullptr;

  std::vector<PointLight> pointLights;
  std::optional<std::size_t> pointLightIndex;
  DirectionalLight directionalLight;

  Scene();

  void draw(const Pipeline& pipeline, const Shader& shader) const;

  void release();

  void setModel(const Model* model);

  void addPointLight(const PointLight& light);

  void setPointLightIndex(std::size_t index);

  void setDirectionalLight(const DirectionalLight& light);
};

}  // namespace ogls