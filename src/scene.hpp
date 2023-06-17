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
 private:
  PointLight pointLight;
  DirectionalLight directionalLight;

 public:
  const Model* model = nullptr;

  Scene();

  void draw(const Pipeline& pipeline, const Shader& shader) const;

  void setModel(const Model* model);

  void setPointLight(const PointLight& light);

  void setDirectionalLight(const DirectionalLight& light);
};

}  // namespace ogls