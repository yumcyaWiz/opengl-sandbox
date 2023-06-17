#pragma once
#include <optional>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
//
#include "model.hpp"
#include "shader.hpp"

namespace ogls
{

class Light
{
 private:
  glm::vec3 ke;

 public:
  Light() : ke{0.0f} {}
  Light(const glm::vec3& ke) : ke{ke} {}

  glm::vec3 getKe() const { return ke; }
};

class PointLight : public Light
{
 private:
  glm::vec3 position;  // for point light
  float radius;        // for point light

 public:
  PointLight() : position{0.0f}, radius{0.0f} {}
  PointLight(const glm::vec3& ke, const glm::vec3& position, float radius)
      : Light{ke}, position{position}, radius{radius}
  {
  }

  glm::vec3 getPosition() const { return position; }
  float getRadius() const { return radius; }
};

class DirectionalLight : public Light
{
 private:
  glm::vec3 direction;  // direction

 public:
  DirectionalLight() : direction{0.0f} {}
  DirectionalLight(const glm::vec3& ke, const glm::vec3& direction)
      : Light{ke}, direction{direction}
  {
  }

  glm::vec3 getDirection() const { return direction; }
};

class Scene
{
 private:
  Model model;
  PointLight pointLight;
  DirectionalLight directionalLight;

 public:
  Scene();
  Scene(const Scene& other) = delete;
  Scene(Scene&& other) = default;
  ~Scene() = default;

  Scene& operator=(const Scene& other) = delete;
  Scene& operator=(Scene&& other) = default;

  void draw(const Pipeline& pipeline, const Shader& shader) const;

  void setModel(Model&& model);

  void setPointLight(const PointLight& light);

  void setDirectionalLight(const DirectionalLight& light);
};

}  // namespace ogls