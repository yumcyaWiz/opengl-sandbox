#pragma once
#include "glad/glad.h"
#include "spdlog/spdlog.h"
//
#include "texture.hpp"

namespace ogls
{

class FrameBuffer
{
 private:
  GLuint framebuffer;
  std::vector<GLenum> attachments;

  void release();

 public:
  FrameBuffer(const std::vector<GLenum>& attachments);
  FrameBuffer(const FrameBuffer& other) = delete;
  FrameBuffer(FrameBuffer&& other);
  ~FrameBuffer();

  FrameBuffer& operator=(const FrameBuffer& other) = delete;
  FrameBuffer& operator=(FrameBuffer&& other);

  void setDrawBuffer(GLenum buf) const;
  void setReadBuffer(GLenum buf) const;

  void bindTexture(const Texture& texture, std::size_t attachment_index) const;

  void activate() const;

  void deactivate() const;
};

}  // namespace ogls