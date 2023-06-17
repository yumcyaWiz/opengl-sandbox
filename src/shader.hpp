#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <variant>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "spdlog/spdlog.h"

namespace ogls
{

class Shader
{
 private:
  GLuint program;

 public:
  Shader();
  Shader(GLenum type, const std::filesystem::path& filepath);
  virtual ~Shader();
  Shader(const Shader& other) = delete;
  Shader(Shader&& other);

  Shader& operator=(const Shader& other) = delete;
  Shader& operator=(Shader&& other);

  void release();

  GLuint getProgram() const;

  void setUniform(const std::string& uniform_name,
                  const std::variant<bool, GLint, GLuint, GLfloat, glm::vec2,
                                     glm::vec3, glm::mat4>& value) const;

  static Shader createVertexShader(const std::filesystem::path& filepath);
  static Shader createFragmentShader(const std::filesystem::path& filepath);
  static Shader createGeometryShader(const std::filesystem::path& filepath);
  static Shader createComputeShader(const std::filesystem::path& filepath);
};

class Pipeline
{
 private:
  GLuint pipeline;

 public:
  Pipeline();
  Pipeline(const Pipeline& other) = delete;
  Pipeline(Pipeline&& other);
  ~Pipeline();

  Pipeline& operator=(const Pipeline& other) = delete;
  Pipeline& operator=(Pipeline&& other);

  void release();

  void attachVertexShader(const Shader& shader) const;
  void attachGeometryShader(const Shader& shader) const;
  void attachFragmentShader(const Shader& shader) const;
  void attachComputeShader(const Shader& shader) const;

  void activate() const;
  void deactivate() const;
};

}  // namespace ogls