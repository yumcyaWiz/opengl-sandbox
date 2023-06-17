#include "scene.hpp"

namespace ogls
{

Scene::Scene() {}

void Scene::draw(const Shader& shader) const
{
  // set point lights
  shader.set_uniform("n_PointLights", static_cast<GLint>(pointLights.size()));
  for (std::size_t i = 0; i < pointLights.size(); ++i) {
    const PointLight& pointLight = pointLights[i];
    const std::string uniformName = "pointLights[" + std::to_string(i) + "]";
    shader.set_uniform(uniformName + ".ke", pointLight.ke);
    shader.set_uniform(uniformName + ".position", pointLight.position);
    shader.set_uniform(uniformName + ".radius", pointLight.radius);
  }

  // set directional light
  shader.set_uniform("directionalLight.ke", directionalLight.ke);
  shader.set_uniform("directionalLight.direction", directionalLight.direction);

  // draw models
  model.draw(shader);
}

void Scene::destroy()
{
  // destroy model
  model.destroy();

  // clear vectors
  pointLights.clear();
}

void Scene::set_model(const Model& model)
{
  // destroy previous model
  if (this->model) { this->model.destroy(); }

  this->model = model;
}

void Scene::add_point_light(const PointLight& light)
{
  pointLights.push_back(light);
}

void Scene::set_point_light_index(std::size_t index)
{
  pointLightIndex = index;
}

void Scene::set_directional_light(const DirectionalLight& light)
{
  directionalLight = light;
}

}  // namespace ogls
