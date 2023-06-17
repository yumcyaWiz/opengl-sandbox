#include "shader.hpp"

#include "Shadinclude.hpp"

using namespace ogls;

std::string Shader::loadStringFromFile(const std::filesystem::path& filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open()) {
    spdlog::error("[Shader] failed to open {}", filepath.generic_string());
    std::exit(EXIT_FAILURE);
  }
  return std::string(std::istreambuf_iterator<char>(file),
                     std::istreambuf_iterator<char>());
}

Shader::Shader() : program(0) {}

Shader::Shader(GLenum type, const std::filesystem::path& filepath)
{
  std::string shader_source = Shadinclude::load(filepath);
  const char* shader_source_c = shader_source.c_str();
  program = glCreateShaderProgramv(type, 1, &shader_source_c);
  spdlog::info("[Shader] program {:x} created", program);

  // check compile and link error
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

Shader::~Shader() { release(); }

Shader::Shader(Shader&& other) : program(other.program) { other.program = 0; }

Shader& Shader::operator=(Shader&& other)
{
  if (this != &other) {
    release();

    program = other.program;

    other.program = 0;
  }

  return *this;
}

void Shader::release()
{
  if (program) {
    spdlog::info("[Shader] release program {:x}", program);
    glDeleteProgram(program);
  }
}

GLuint Shader::getProgram() const { return program; }

void Shader::setUniform(
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

Shader Shader::create_vertex_shader(const std::filesystem::path& filepath)
{
  return Shader(GL_VERTEX_SHADER, filepath);
}

Shader Shader::create_fragment_shader(const std::filesystem::path& filepath)
{
  return Shader(GL_FRAGMENT_SHADER, filepath);
}

Shader Shader::create_geometry_shader(const std::filesystem::path& filepath)
{
  return Shader(GL_GEOMETRY_SHADER, filepath);
}

Shader Shader::create_compute_shader(const std::filesystem::path& filepath)
{
  return Shader(GL_COMPUTE_SHADER, filepath);
}

Pipeline::Pipeline()
{
  glCreateProgramPipelines(1, &pipeline);
  spdlog::info("[Pipeline] pipeline {:x} created", pipeline);
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
    spdlog::info("[Pipeline] release pipeline {:x}", pipeline);
    glDeleteProgramPipelines(1, &pipeline);
  }
}

void Pipeline::attachVertexShader(const Shader& shader) const
{
  glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, shader.getProgram());
}

void Pipeline::attachGeometryShader(const Shader& shader) const
{
  glUseProgramStages(pipeline, GL_GEOMETRY_SHADER_BIT, shader.getProgram());
}

void Pipeline::attachFragmentShader(const Shader& shader) const
{
  glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, shader.getProgram());
}

void Pipeline::attachComputeShader(const Shader& shader) const
{
  glUseProgramStages(pipeline, GL_COMPUTE_SHADER_BIT, shader.getProgram());
}

void Pipeline::activate() const { glBindProgramPipeline(pipeline); }

void Pipeline::deactivate() const { glBindProgramPipeline(0); }