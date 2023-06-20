#include "buffer.hpp"

using namespace ogls;

Buffer::Buffer() : buffer{0}, size{0}
{
    glCreateBuffers(1, &buffer);

    spdlog::debug("[Buffer] created buffer {:x}", buffer);
}

Buffer::Buffer(Buffer&& other) : buffer(other.buffer), size(other.size)
{
    other.buffer = 0;
}

Buffer::~Buffer() { release(); }

Buffer& Buffer::operator=(Buffer&& other)
{
    if (this != &other) {
        release();

        buffer = other.buffer;
        size = other.size;

        other.buffer = 0;
    }

    return *this;
}

void Buffer::release()
{
    if (buffer) {
        spdlog::debug("[Buffer] release buffer {:x}", buffer);

        glDeleteBuffers(1, &buffer);
        this->buffer = 0;
    }
}

GLuint Buffer::getName() const { return buffer; }

uint32_t Buffer::getLength() const { return size; }

void Buffer::bindToShaderStorageBuffer(GLuint binding_point_index) const
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point_index, buffer);
}