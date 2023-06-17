#include "scene.hpp"

namespace ogls
{

Scene::Scene() {}

void Scene::draw(const Pipeline& pipeline, const Shader& shader) const
{
  // set point light
  shader.setUniform("pointLight.ke", pointLight.getKe());
  shader.setUniform("pointLight.position", pointLight.getPosition());
  shader.setUniform("pointLight.radius", pointLight.getRadius());

  // set directional light
  shader.setUniform("directionalLight.ke", directionalLight.getKe());
  shader.setUniform("directionalLight.direction",
                    directionalLight.getDirection());

  // draw models
  if (model) { model->draw(pipeline, shader); }
}

void Scene::setModel(const Model* model) { this->model = model; }

void Scene::setPointLight(const PointLight& light) { pointLight = light; }

void Scene::setDirectionalLight(const DirectionalLight& light)
{
  directionalLight = light;
}

}  // namespace ogls
