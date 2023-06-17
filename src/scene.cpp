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
    shader.setUniform(uniformName + ".ke", pointLight.getKe());
    shader.setUniform(uniformName + ".position", pointLight.getPosition());
    shader.setUniform(uniformName + ".radius", pointLight.getRadius());
  }

  // set directional light
  shader.setUniform("directionalLight.ke", directionalLight.getKe());
  shader.setUniform("directionalLight.direction",
                    directionalLight.getDirection());

  // draw models
  if (model) { model->draw(pipeline, shader); }
}

void Scene::release()
{
  // clear vectors
  pointLights.clear();
}

void Scene::setModel(const Model* model) { this->model = model; }

void Scene::addPointLight(const PointLight& light)
{
  pointLights.push_back(light);
}

void Scene::setPointLightIndex(std::size_t index) { pointLightIndex = index; }

void Scene::setDirectionalLight(const DirectionalLight& light)
{
  directionalLight = light;
}

}  // namespace ogls
