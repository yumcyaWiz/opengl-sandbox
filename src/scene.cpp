#include "scene.hpp"

namespace ogls
{

Scene::Scene() {}

void Scene::draw(const Pipeline& pipeline, const Shader& shader) const
{
  // set point lights
  shader.setUniform("n_PointLights", static_cast<GLint>(pointLights.size()));
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
  if (model) { model->draw(pipeline, shader); }
}

void Scene::destroy()
{
  // clear vectors
  pointLights.clear();
}

void Scene::set_model(const Model* model) { this->model = model; }

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
