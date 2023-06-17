#include "scene.hpp"

namespace ogls
{

Scene::Scene() {}

void Scene::draw(const Pipeline& pipeline) const
{
  // set point light
  pipeline.setUniform("pointLight.ke", pointLight.getKe());
  pipeline.setUniform("pointLight.position", pointLight.getPosition());
  pipeline.setUniform("pointLight.radius", pointLight.getRadius());

  // set directional light
  pipeline.setUniform("directionalLight.ke", directionalLight.getKe());
  pipeline.setUniform("directionalLight.direction",
                      directionalLight.getDirection());

  // draw models
  if (model) { model.draw(pipeline); }
}

void Scene::setModel(Model&& model) { this->model = std::move(model); }

void Scene::setPointLight(const PointLight& light) { pointLight = light; }

void Scene::setDirectionalLight(const DirectionalLight& light)
{
  directionalLight = light;
}

}  // namespace ogls
