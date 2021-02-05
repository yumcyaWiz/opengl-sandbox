#include "ogls/scene.hpp"

namespace ogls {

Scene::Scene() {}

void Scene::draw(const Shader& shader) const {
  // set point lights
  for (std::size_t i = 0; i < pointLights.size(); ++i) {
    const PointLight& pointLight = pointLights[i];
    const std::string uniformName = "pointLights[" + std::to_string(i) + "]";
    shader.setUniform(uniformName + ".ke", pointLight.ke);
    shader.setUniform(uniformName + ".position", pointLight.position);
    shader.setUniform(uniformName + ".radius", pointLight.radius);
  }

  // set directional light
  shader.setUniform("directionalLight.ke", directionalLight.ke);
  shader.setUniform("directionalLight.direction", directionalLight.direction);

  // draw models
  model.draw(shader);
}

void Scene::destroy() {
  // destroy model
  model.destroy();

  // clear vectors
  pointLights.clear();
}

void Scene::setModel(const Model& model) {
  // destroy previous model
  if (this->model) {
    this->model.destroy();
  }

  this->model = model;
}

void Scene::addPointLight(const PointLight& light) {
  pointLights.push_back(light);
}

void Scene::setDirectionalLight(const DirectionalLight& light) {
  directionalLight = light;
}

}  // namespace ogls
