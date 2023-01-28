#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <variant>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace ogls
{

class Shader
{
 private:
  std::filesystem::path vertexShaderFilepath;
  std::string vertexShaderSource;

  std::optional<std::filesystem::path> geometryShaderFilepath;
  std::optional<std::string> geometryShaderSource;

  std::filesystem::path fragmentShaderFilepath;
  std::string fragmentShaderSource;

  GLuint vertexShader;
  std::optional<GLuint> geometryShader;
  GLuint fragmentShader;
  GLuint program;

  void checkShaderCompilation(GLuint shader);

 public:
  Shader();

  void load_vertex_shader(const std::filesystem::path& vertexShaderFilepath);
  void load_geometry_shader(
      const std::filesystem::path& geometryShaderFilepath);
  void load_fragment_shader(
      const std::filesystem::path& fragmentShaderFilepath);

  void link_shader();

  // destroy shader object
  void destroy();

  // activate shader on the currect context
  void activate() const;
  // deactivate shader on the currect context
  void deactivate() const;

  // set value on shader's uniform variable
  void set_uniform(const std::string& uniformName,
                   const std::variant<bool, GLint, GLuint, GLfloat, glm::vec2,
                                      glm::vec3, glm::mat4>& value) const;

  // set texture on shader's sampler2D uniform variable
  void set_uniform_texture(const std::string& uniformName, GLuint texture,
                           GLuint textureUnitNumber) const;

  // set cubemap on shader's samplerCube uniform variable
  void set_uniform_cubemap(const std::string& uniformName, GLuint cubemap,
                           GLuint textureUnitNumber) const;

  // set uniform buffer object on shader
  void set_UBO(const std::string& blockName, GLuint bindingNumber) const;
};

}  // namespace ogls