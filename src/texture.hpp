#pragma once
#include <filesystem>
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "spdlog/spdlog.h"

namespace ogls
{

enum class TextureType {
  Diffuse,
  Specular,
  Ambient,
  Emissive,
  Height,
  Normal,
  Shininess,
  Displacement,
  Light
};

class Texture
{
 public:
  class TextureBuilder
  {
   private:
    glm::uvec2 resolution = {0, 0};

    GLint internalFormat = GL_SRGB;
    GLenum format = GL_RGB;
    GLenum type = GL_UNSIGNED_BYTE;
    GLenum wrap_s = GL_REPEAT;
    GLenum wrap_t = GL_REPEAT;
    GLenum mag_filter = GL_LINEAR;
    GLenum min_filter = GL_LINEAR;
    bool generate_mipmap = false;
    bool depth_compare_mode = false;

    const void* image = nullptr;

   public:
    TextureBuilder(const glm::vec2& resolution) : resolution(resolution) {}

    TextureBuilder setInternalFormat(GLint internalFormat)
    {
      this->internalFormat = internalFormat;
      return *this;
    }

    TextureBuilder setFormat(GLenum format)
    {
      this->format = format;
      return *this;
    }

    TextureBuilder setType(GLenum type)
    {
      this->type = type;
      return *this;
    }

    TextureBuilder setWrapS(GLenum wrap_s)
    {
      this->wrap_s = wrap_s;
      return *this;
    }

    TextureBuilder setWrapT(GLenum wrap_t)
    {
      this->wrap_t = wrap_t;
      return *this;
    }

    TextureBuilder setMagFilter(GLenum mag_filter)
    {
      this->mag_filter = mag_filter;
      return *this;
    }

    TextureBuilder setMinFilter(GLenum min_filter)
    {
      this->min_filter = min_filter;
      return *this;
    }

    TextureBuilder setGenerateMipmap(bool generate_mipmap)
    {
      this->generate_mipmap = generate_mipmap;
      return *this;
    }

    TextureBuilder setImage(const void* image)
    {
      this->image = image;
      return *this;
    }

    TextureBuilder setDepthCompareMode(bool depth_compare_mode)
    {
      this->depth_compare_mode = depth_compare_mode;
      return *this;
    }

    Texture build() const { return Texture(*this); }

    friend class Texture;
  };

  Texture();
  Texture(const Texture& other) = delete;
  Texture(Texture&& other);
  ~Texture();

  Texture& operator=(const Texture& other) = delete;
  Texture& operator=(Texture&& other);

  glm::uvec2 getResolution() const;
  GLuint getTextureName() const;
  GLint getInternalFormat() const;
  GLenum getFormat() const;
  GLenum getType() const;

  // bind texture to the specified texture unit
  void bindToTextureUnit(GLuint texture_unit_number) const;

  // bind texture to the specified image unit
  void bindToImageUnit(GLuint image_unit_number, GLenum access) const;

 private:
  Texture(const TextureBuilder& builder);

  void createTexture();

  void setImage(const void* image) const;

  glm::uvec2 resolution;

  GLuint texture;
  GLint internalFormat;
  GLenum format;
  GLenum type;
  GLenum wrap_s;
  GLenum wrap_t;
  GLenum mag_filter;
  GLenum min_filter;
  bool generate_mipmap;
  bool depth_compare_mode;

  void release();
};

}  // namespace ogls