#include "shader.hpp"

#include "Shadinclude.hpp"

using namespace ogls;

Pipeline::Shader::Shader() : program(0) {}

GLuint Pipeline::Shader::createShaderProgram(
    GLenum type, const std::filesystem::path& filepath)
{
  std::string shader_source = Shadinclude::load(filepath);
  const char* shader_source_c = shader_source.c_str();
  GLuint program = glCreateShaderProgramv(type, 1, &shader_source_c);
  spdlog::debug("[Shader] program {:x} created", program);
  return program;
}

void Pipeline::Shader::checkCompileError(GLuint program)
{
  int success = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    spdlog::error("[Shader] failed to link program {:x}", program);

    GLint logSize = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    std::vector<GLchar> errorLog(logSize);
    glGetProgramInfoLog(program, logSize, &logSize, &errorLog[0]);
    std::string errorLogStr(errorLog.begin(), errorLog.end());
    spdlog::error("[Shader] {}", errorLogStr);
  }
}

Pipeline::Shader::Shader(GLenum type, const std::filesystem::path& filepath)
{
  program = createShaderProgram(type, filepath);
  checkCompileError(program);
}

Pipeline::Shader::~Shader() { release(); }

Pipeline::Shader::Shader(Shader&& other) : program(other.program)
{
  other.program = 0;
}

Pipeline::Shader& Pipeline::Shader::operator=(Shader&& other)
{
  if (this != &other) {
    release();

    program = other.program;

    other.program = 0;
  }

  return *this;
}

Pipeline::Shader::operator bool() const { return this->program != 0; }

void Pipeline::Shader::release()
{
  if (program) {
    spdlog::debug("[Shader] release program {:x}", program);
    glDeleteProgram(program);
  }
}

GLuint Pipeline::Shader::getProgram() const { return program; }

void Pipeline::Shader::setUniform(
    const std::string& uniform_name,
    const std::variant<bool, GLint, GLuint, GLfloat, glm::vec2, glm::vec3,
                       glm::mat4>& value) const
{
  // get location of uniform variable
  const GLint location = glGetUniformLocation(program, uniform_name.c_str());

  // set value
  struct Visitor {
    GLuint program;
    GLint location;
    Visitor(GLuint program, GLint location)
        : program(program), location(location)
    {
    }

    void operator()(bool value)
    {
      glProgramUniform1i(program, location, value);
    }
    void operator()(GLint value)
    {
      glProgramUniform1i(program, location, value);
    }
    void operator()(GLuint value)
    {
      glProgramUniform1ui(program, location, value);
    }
    void operator()(GLfloat value)
    {
      glProgramUniform1f(program, location, value);
    }
    void operator()(const glm::vec2& value)
    {
      glProgramUniform2fv(program, location, 1, glm::value_ptr(value));
    }
    void operator()(const glm::vec3& value)
    {
      glProgramUniform3fv(program, location, 1, glm::value_ptr(value));
    }
    void operator()(const glm::mat4& value)
    {
      glProgramUniformMatrix4fv(program, location, 1, GL_FALSE,
                                glm::value_ptr(value));
    }
  };
  std::visit(Visitor{program, location}, value);
}

Pipeline::Shader Pipeline::Shader::createVertexShader(
    const std::filesystem::path& filepath)
{
  return Shader(GL_VERTEX_SHADER, filepath);
}

Pipeline::Shader Pipeline::Shader::createFragmentShader(
    const std::filesystem::path& filepath)
{
  return Shader(GL_FRAGMENT_SHADER, filepath);
}

Pipeline::Shader Pipeline::Shader::createGeometryShader(
    const std::filesystem::path& filepath)
{
  return Shader(GL_GEOMETRY_SHADER, filepath);
}

Pipeline::Shader Pipeline::Shader::createComputeShader(
    const std::filesystem::path& filepath)
{
  return Shader(GL_COMPUTE_SHADER, filepath);
}

Pipeline::Pipeline()
{
  glCreateProgramPipelines(1, &pipeline);
  spdlog::debug("[Pipeline] pipeline {:x} created", pipeline);
}

Pipeline::Pipeline(Pipeline&& other) : pipeline(other.pipeline)
{
  other.pipeline = 0;
}

Pipeline::~Pipeline() { release(); }

Pipeline& Pipeline::operator=(Pipeline&& other)
{
  if (this != &other) {
    release();

    pipeline = other.pipeline;

    other.pipeline = 0;
  }

  return *this;
}

void Pipeline::release()
{
  if (pipeline) {
    spdlog::debug("[Pipeline] release pipeline {:x}", pipeline);
    glDeleteProgramPipelines(1, &pipeline);
  }
}

void Pipeline::attachVertexShader(Shader&& shader)
{
  vertex_shader = std::move(shader);
  glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT,
                     vertex_shader.getProgram());
}

void Pipeline::attachGeometryShader(Shader&& shader)
{
  geometry_shader = std::move(shader);
  glUseProgramStages(pipeline, GL_GEOMETRY_SHADER_BIT,
                     geometry_shader.getProgram());
}

void Pipeline::attachFragmentShader(Shader&& shader)
{
  fragment_shader = std::move(shader);
  glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT,
                     fragment_shader.getProgram());
}

void Pipeline::attachComputeShader(Shader&& shader)
{
  compute_shader = std::move(shader);
  glUseProgramStages(pipeline, GL_COMPUTE_SHADER_BIT,
                     compute_shader.getProgram());
}

void Pipeline::loadVertexShader(const std::filesystem::path& filepath)
{
  attachVertexShader(Shader::createVertexShader(filepath));
}

void Pipeline::loadFragmentShader(const std::filesystem::path& filepath)
{
  attachFragmentShader(Shader::createFragmentShader(filepath));
}

void Pipeline::loadGeometryShader(const std::filesystem::path& filepath)
{
  attachGeometryShader(Shader::createGeometryShader(filepath));
}

void Pipeline::loadComputeShader(const std::filesystem::path& filepath)
{
  attachComputeShader(Shader::createComputeShader(filepath));
}

void Pipeline::setUniform(
    const std::string& uniform_name,
    const std::variant<bool, GLint, GLuint, GLfloat, glm::vec2, glm::vec3,
                       glm::mat4>& value) const
{
  if (vertex_shader) { vertex_shader.setUniform(uniform_name, value); }
  if (geometry_shader) { geometry_shader.setUniform(uniform_name, value); }
  if (fragment_shader) { fragment_shader.setUniform(uniform_name, value); }
  if (compute_shader) { compute_shader.setUniform(uniform_name, value); }
}

void Pipeline::activate() const { glBindProgramPipeline(pipeline); }

void Pipeline::deactivate() const { glBindProgramPipeline(0); }