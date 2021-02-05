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
  for (const auto& model : models) {
    model.draw(shader);
  }
}

void Scene::destroy() {
  // destroy each models
  for (auto& model : models) {
    model.destroy();
  }

  // clear vectors
  models.clear();
  pointLights.clear();
}

void Scene::addModel(const Model& model) { models.push_back(model); }

void Scene::addPointLight(const PointLight& light) {
  pointLights.push_back(light);
}

void Scene::setDirectionalLight(const DirectionalLight& light) {
  directionalLight = light;
}

}  // namespace ogls
