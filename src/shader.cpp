#include "ogls/shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Shadinclude.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "spdlog/spdlog.h"

namespace ogls {

Shader::Shader() : vertexShader{0}, fragmentShader{0}, program{0} {}

void Shader::checkShaderCompilation(GLuint shader) {
  int success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    spdlog::error("[Shader] failed to compile shader");

    GLint logSize = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    std::vector<GLchar> errorLog(logSize);
    glGetShaderInfoLog(shader, logSize, &logSize, errorLog.data());
    std::string errorLogStr(errorLog.begin(), errorLog.end());
    spdlog::error(errorLogStr);
  }
}

void Shader::setVertexShader(
    const std::filesystem::path& vertexShaderFilepath) {
  this->vertexShaderFilepath = vertexShaderFilepath;

  // delete previous shader
  if (vertexShader) {
    glDeleteShader(vertexShader);
  }

  spdlog::info("[Shader] loading vertex shader from " +
               vertexShaderFilepath.string());

  // compile vertex shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  vertexShaderSource = Shadinclude::load(vertexShaderFilepath.string());
  if (vertexShaderSource.size() == 0) {
    spdlog::error("failed to open " + vertexShaderFilepath.string());
  }
  const char* vertexShaderSourceC = vertexShaderSource.c_str();
  glShaderSource(vertexShader, 1, &vertexShaderSourceC, nullptr);
  glCompileShader(vertexShader);

  checkShaderCompilation(vertexShader);
}

void Shader::setGeometryShader(
    const std::filesystem::path& geometryShaderFilepath) {
  this->geometryShaderFilepath = geometryShaderFilepath;

  // delete previous shader
  if (geometryShader) {
    glDeleteShader(geometryShader.value());
  }

  spdlog::info("[Shader] loading geometry shader from " +
               geometryShaderFilepath.string());

  geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  geometryShaderSource = Shadinclude::load(geometryShaderFilepath.string());
  if (geometryShaderSource.value().size() == 0) {
    spdlog::error("failed to open " + geometryShaderFilepath.string());
  }
  const char* geometryShaderSourceC = geometryShaderSource.value().c_str();
  glShaderSource(geometryShader.value(), 1, &geometryShaderSourceC, nullptr);
  glCompileShader(geometryShader.value());

  checkShaderCompilation(geometryShader.value());
}

void Shader::setFragmentShader(
    const std::filesystem::path& fragmentShaderFilepath) {
  this->fragmentShaderFilepath = fragmentShaderFilepath;

  // delete previous shader
  if (fragmentShader) {
    glDeleteShader(fragmentShader);
  }

  spdlog::info("[Shader] loading fragment shader from " +
               fragmentShaderFilepath.string());

  // compile fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  fragmentShaderSource = Shadinclude::load(fragmentShaderFilepath.string());
  if (fragmentShaderSource.size() == 0) {
    spdlog::error("failed to open " + fragmentShaderFilepath.string());
  }
  const char* fragmentShaderSourceC = fragmentShaderSource.c_str();
  glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, nullptr);
  glCompileShader(fragmentShader);

  checkShaderCompilation(fragmentShader);
}

void Shader::destroy() {
  if (vertexShader) {
    glDeleteShader(vertexShader);
  }

  if (geometryShader) {
    glDeleteShader(geometryShader.value());
  }

  if (fragmentShader) {
    glDeleteShader(fragmentShader);
  }

  if (program) {
    glDeleteProgram(program);
  }
}

void Shader::activate() const { glUseProgram(program); }

void Shader::deactivate() const { glUseProgram(0); }

void Shader::setUniform(
    const std::string& uniformName,
    const std::variant<bool, GLint, GLuint, GLfloat, glm::vec2, glm::vec3,
                       glm::mat4>& value) const {
  activate();

  // get location of uniform variable
  const GLint location = glGetUniformLocation(program, uniformName.c_str());

  // set value
  struct Visitor {
    GLint location;
    Visitor(GLint location) : location(location) {}

    void operator()(bool value) { glUniform1i(location, value); }
    void operator()(GLint value) { glUniform1i(location, value); }
    void operator()(GLuint value) { glUniform1ui(location, value); }
    void operator()(GLfloat value) { glUniform1f(location, value); }
    void operator()(const glm::vec2& value) {
      glUniform2fv(location, 1, glm::value_ptr(value));
    }
    void operator()(const glm::vec3& value) {
      glUniform3fv(location, 1, glm::value_ptr(value));
    }
    void operator()(const glm::mat4& value) {
      glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
  };
  std::visit(Visitor{location}, value);

  deactivate();
}

void Shader::setUBO(const std::string& blockName, GLuint bindingNumber) const {
  const GLuint blockIndex = glGetUniformBlockIndex(program, blockName.c_str());

  // set binding number of specified block
  glUniformBlockBinding(program, blockIndex, bindingNumber);
}

void Shader::setUniformTexture(const std::string& uniformName, GLuint texture,
                               GLuint textureUnitNumber) const {
  activate();

  // bind texture to specified texture unit
  glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
  glBindTexture(GL_TEXTURE_2D, texture);

  // set texture unit number on uniform variable
  const GLint location = glGetUniformLocation(program, uniformName.c_str());
  glUniform1i(location, textureUnitNumber);

  deactivate();
}

void Shader::setUniformCubemap(const std::string& uniformName, GLuint cubemap,
                               GLuint textureUnitNumber) const {
  activate();

  // bind cubemap to specified texture unit
  glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

  // set texture unit number on uniform variable
  const GLint location = glGetUniformLocation(program, uniformName.c_str());
  glUniform1i(location, textureUnitNumber);

  deactivate();
}

void Shader::linkShader() {
  // link shader program
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  if (geometryShader) {
    glAttachShader(program, geometryShader.value());
  }
  glAttachShader(program, fragmentShader);

  glLinkProgram(program);

  glDetachShader(program, vertexShader);
  if (geometryShader) {
    glDetachShader(program, geometryShader.value());
  }
  glDetachShader(program, fragmentShader);

  // handle link error
  int success = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    spdlog::error("failed to link shaders");

    GLint logSize = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    std::vector<GLchar> errorLog(logSize);
    glGetProgramInfoLog(program, logSize, &logSize, &errorLog[0]);
    std::string errorLogStr(errorLog.begin(), errorLog.end());
    spdlog::error(errorLogStr);

    glDeleteProgram(program);
  }
}

}  // namespace ogls