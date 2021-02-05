#ifndef _OGLS_SCENE_H
#define _OGLS_SCENE_H
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
  Scene();

  // draw scene
  void draw(const Shader& shader) const;

  // destroy scene object
  void destroy();

  void setModel(const Model& model);

  void addPointLight(const PointLight& light);

  void setDirectionalLight(const DirectionalLight& light);

 private:
  Model model;

  std::vector<PointLight> pointLights;
  DirectionalLight directionalLight;
};

}  // namespace ogls

#endif