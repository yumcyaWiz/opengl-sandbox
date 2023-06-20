#pragma once

#include "glad/glad.h"
#include "spdlog/spdlog.h"
//
#include "buffer.hpp"

namespace ogls
{

class VertexArrayObject
{
   private:
    GLuint array;

   public:
    VertexArrayObject();
    VertexArrayObject(const VertexArrayObject& other) = delete;
    VertexArrayObject(VertexArrayObject&& other);
    ~VertexArrayObject();

    VertexArrayObject& operator=(const VertexArrayObject& other) = delete;
    VertexArrayObject& operator=(VertexArrayObject&& other);

    void bindVertexBuffer(const Buffer& buffer, GLuint binding, GLintptr offset,
                          GLsizei stride) const;

    void bindElementBuffer(const Buffer& buffer) const;

    void activateVertexAttribution(GLuint binding, GLuint attrib, GLint size,
                                   GLenum type, GLsizei offset) const;

    void activate() const;

    void deactivate() const;

    void release();
};

}  // namespace ogls