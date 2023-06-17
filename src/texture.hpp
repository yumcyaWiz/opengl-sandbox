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
 private:
  glm::uvec2 resolution;
  GLuint texture;
  GLint internalFormat;
  GLenum format;
  GLenum type;

  void release();

 public:
  Texture();
  Texture(const glm::uvec2& resolution, GLint internalFormat, GLenum format,
          GLenum type);
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

  void initImage(const glm::uvec2& resolution, GLint internalFormat,
                 GLenum format, GLenum type);

  template <typename T>
  void setImage(const T* image, const glm::uvec2& resolution,
                GLint internalFormat, GLenum format, GLenum type)
  {
    this->resolution = resolution;
    this->internalFormat = internalFormat;
    this->format = format;
    this->type = type;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, resolution.x, resolution.y,
                 0, format, type, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void resize(const glm::uvec2& resolution);

  // bind texture to the specified texture unit
  void bindToTextureUnit(GLuint texture_unit_number) const;

  // bind texture to the specified image unit
  void bindToImageUnit(GLuint image_unit_number, GLenum access) const;
};

}  // namespace ogls