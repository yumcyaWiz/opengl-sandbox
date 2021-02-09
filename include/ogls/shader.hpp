#ifndef _OGLS_SHADER_H
#define _OGLS_SHADER_H

#include <filesystem>
#include <optional>
#include <string>
#include <variant>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace ogls {

class Shader {
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

  void setVertexShader(const std::filesystem::path& vertexShaderFilepath);
  void setGeometryShader(const std::filesystem::path& geometryShaderFilepath);
  void setFragmentShader(const std::filesystem::path& fragmentShaderFilepath);

  void linkShader();

  // destroy shader object
  void destroy();

  // activate shader on the currect context
  void activate() const;
  // deactivate shader on the currect context
  void deactivate() const;

  // set value on shader's uniform variable
  void setUniform(const std::string& uniformName,
                  const std::variant<bool, GLint, GLuint, GLfloat, glm::vec2,
                                     glm::vec3, glm::mat4>& value) const;

  // set texture on shader's sampler2D uniform variable
  void setUniformTexture(const std::string& uniformName, GLuint texture,
                         GLuint textureUnitNumber) const;

  // set uniform buffer object on shader
  void setUBO(const std::string& blockName, GLuint bindingNumber) const;
};

}  // namespace ogls

#endif