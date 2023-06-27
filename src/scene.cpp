#include "scene.hpp"

namespace ogls
{

Scene::Scene() {}

void Scene::init()
{
    constexpr uint8_t black_pixel[3] = {0, 0, 0};

    null_texture = Texture::TextureBuilder({1, 1})
                       .setInternalFormat(GL_RGB8)
                       .setFormat(GL_RGB)
                       .setType(GL_UNSIGNED_BYTE)
                       .setImage(&black_pixel[0])
                       .build();
}

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
    if (model) { model.draw(pipeline, null_texture); }
}

void Scene::setModel(Model&& model) { this->model = std::move(model); }

void Scene::setPointLight(const PointLight& light) { pointLight = light; }

void Scene::setDirectionalLight(const DirectionalLight& light)
{
    directionalLight = light;
}

}  // namespace ogls
