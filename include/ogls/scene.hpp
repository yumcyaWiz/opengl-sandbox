#ifndef _OGLS_SCENE_H
#define _OGLS_SCENE_H
#include <optional>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
//
#include "light.hpp"
#include "model.hpp"
#include "shader.hpp"

namespace ogls {

class Scene {
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

  void setModel(const Model& model);

  void addPointLight(const PointLight& light);

  void setPointLightIndex(std::size_t index);

  void setDirectionalLight(const DirectionalLight& light);
};

}  // namespace ogls

#endif