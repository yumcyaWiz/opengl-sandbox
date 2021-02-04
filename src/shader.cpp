#include "rtgs/shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "glm/gtc/type_ptr.hpp"

namespace rtgs {

Shader::Shader() {}

Shader::Shader(const std::string& vertexShaderFilepath,
               const std::string& fragmentShaderFilepath)
    : vertexShaderFilepath(vertexShaderFilepath),
      fragmentShaderFilepath(fragmentShaderFilepath) {
  std::cout << "[Shader] loading " << vertexShaderFilepath << std::endl;
  std::cout << "[Shader] loading " << fragmentShaderFilepath << std::endl;
  compileShader();
  linkShader();
}

void Shader::destroy() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(program);
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

std::string Shader::fileToString(const std::string& filepath) {
  std::ifstream file(filepath);
  if (!file) {
    std::cerr << "failed to open " << filepath << std::endl;
    return "";
  }

  std::stringstream ss;
  ss << file.rdbuf();
  file.close();

  return ss.str();
}

void Shader::compileShader() {
  // compile vertex shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  vertexShaderSource = fileToString(vertexShaderFilepath);
  const char* vertexShaderSourceC = vertexShaderSource.c_str();
  glShaderSource(vertexShader, 1, &vertexShaderSourceC, nullptr);
  glCompileShader(vertexShader);

  // handle compilation error
  GLint success = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    std::cerr << "failed to compile vertex shader" << std::endl;

    GLint logSize = 0;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
    std::vector<GLchar> errorLog(logSize);
    glGetShaderInfoLog(vertexShader, logSize, &logSize, errorLog.data());
    std::string errorLogStr(errorLog.begin(), errorLog.end());
    std::cerr << errorLogStr << std::endl;

    glDeleteShader(vertexShader);
    return;
  }

  // compile fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  fragmentShaderSource = fileToString(fragmentShaderFilepath);
  const char* fragmentShaderSourceC = fragmentShaderSource.c_str();
  glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, nullptr);
  glCompileShader(fragmentShader);

  // handle compilation error
  success = 0;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    std::cerr << "failed to compile vertex shader" << std::endl;

    GLint logSize = 0;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
    std::vector<GLchar> errorLog(logSize);
    glGetShaderInfoLog(fragmentShader, logSize, &logSize, errorLog.data());
    std::string errorLogStr(errorLog.begin(), errorLog.end());
    std::cerr << errorLogStr << std::endl;

    glDeleteShader(fragmentShader);
    return;
  }
}

void Shader::linkShader() {
  // link shader program
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);

  // handle link error
  int success = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    std::cerr << "failed to link shaders" << std::endl;

    GLint logSize = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    std::vector<GLchar> errorLog(logSize);
    glGetProgramInfoLog(program, logSize, &logSize, &errorLog[0]);
    std::string errorLogStr(errorLog.begin(), errorLog.end());
    std::cerr << errorLogStr << std::endl;

    glDeleteProgram(program);
    return;
  }
}

}  // namespace rtgs