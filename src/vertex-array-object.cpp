#include "vertex-array-object.hpp"

using namespace ogls;

VertexArrayObject::VertexArrayObject()
{
  glCreateVertexArrays(1, &array);

  spdlog::debug("[VertexArrayObject] VAO {:x} created", array);
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& other)
    : array(other.array)
{
  other.array = 0;
}

VertexArrayObject::~VertexArrayObject() { release(); }

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other)
{
  if (this != &other) {
    release();

    array = other.array;

    other.array = 0;
  }

  return *this;
}

void VertexArrayObject::bindVertexBuffer(const Buffer& buffer, GLuint binding,
                                         GLintptr offset, GLsizei stride) const
{
  glVertexArrayVertexBuffer(array, binding, buffer.getName(), offset, stride);
}

void VertexArrayObject::bindElementBuffer(const Buffer& buffer) const
{
  glVertexArrayElementBuffer(array, buffer.getName());
}

void VertexArrayObject::activateVertexAttribution(GLuint binding, GLuint attrib,
                                                  GLint size, GLenum type,
                                                  GLsizei offset) const
{
  glEnableVertexArrayAttrib(array, attrib);
  glVertexArrayAttribBinding(array, attrib, binding);
  glVertexArrayAttribFormat(array, attrib, size, type, GL_FALSE, offset);
}

void VertexArrayObject::activate() const { glBindVertexArray(array); }

void VertexArrayObject::deactivate() const { glBindVertexArray(0); }

void VertexArrayObject::release()
{
  if (array) {
    spdlog::debug("[VertexArrayObject] release VAO {:x}", array);
    glDeleteVertexArrays(1, &array);
  }
}