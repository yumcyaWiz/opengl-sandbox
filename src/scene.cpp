#include "ogls/scene.hpp"

namespace ogls {

Scene::Scene() {}

void Scene::draw(const Shader& shader) const {
  // set point light
  if (pointLightIndex) {
    const PointLight& pointLight = pointLights[pointLightIndex.value()];
    shader.setUniform("pointLight.ke", pointLight.ke);
    shader.setUniform("pointLight.position", pointLight.position);
    shader.setUniform("pointLight.radius", pointLight.radius);
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

void Scene::setPointLightIndex(std::size_t index) { pointLightIndex = index; }

void Scene::setDirectionalLight(const DirectionalLight& light) {
  directionalLight = light;
}

}  // namespace ogls
