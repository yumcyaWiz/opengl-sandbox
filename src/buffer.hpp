#pragma once
#include <vector>

#include "glad/glad.h"
#include "spdlog/spdlog.h"

namespace ogls
{

class Buffer
{
 private:
  GLuint buffer;
  uint32_t size;

  void release();

 public:
  Buffer();
  Buffer(const Buffer& buffer) = delete;
  Buffer(Buffer&& other);
  ~Buffer();

  Buffer& operator=(const Buffer& buffer) = delete;
  Buffer& operator=(Buffer&& other);

  GLuint getName() const;

  uint32_t getLength() const;

  template <typename T>
  void setData(const T* data, uint32_t n, GLenum usage)
  {
    glNamedBufferData(this->buffer, sizeof(T) * n, data, usage);
    this->size = n;
  }

  template <typename T>
  void setData(const std::vector<T>& data, GLenum usage)
  {
    glNamedBufferData(this->buffer, sizeof(T) * data.size(), data.data(),
                      usage);
    this->size = data.size();
  }

  void bindToShaderStorageBuffer(GLuint binding_point_index) const;
};

}  // namespace ogls