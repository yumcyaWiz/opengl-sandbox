#include "texture.hpp"

using namespace ogls;

Texture::Texture()
{
  // init texture
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

  spdlog::debug("[Texture] texture {:x} created", this->texture);
}

Texture::Texture(const glm::uvec2& resolution, GLint internalFormat,
                 GLenum format, GLenum type)
    : Texture()
{
  initImage(resolution, internalFormat, format, type);
}

Texture::Texture(Texture&& other)
    : resolution(other.resolution),
      texture(other.texture),
      internalFormat(other.internalFormat),
      format(other.format),
      type(other.type)
{
  other.texture = 0;
}

Texture::~Texture() { release(); }

Texture& Texture::operator=(Texture&& other)
{
  if (this != &other) {
    release();

    resolution = std::move(other.resolution);
    texture = other.texture;
    internalFormat = other.internalFormat;
    format = other.format;
    type = other.type;

    other.texture = 0;
  }

  return *this;
}

glm::uvec2 Texture::getResolution() const { return this->resolution; }

GLuint Texture::getTextureName() const { return this->texture; }

GLint Texture::getInternalFormat() const { return this->internalFormat; }

GLenum Texture::getFormat() const { return this->format; }

GLenum Texture::getType() const { return this->type; }

void Texture::initImage(const glm::uvec2& resolution, GLint internalFormat,
                        GLenum format, GLenum type)
{
  this->resolution = resolution;
  this->internalFormat = internalFormat;
  this->format = format;
  this->type = type;

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, resolution.x, resolution.y, 0,
               format, type, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::resize(const glm::uvec2& resolution)
{
  initImage(resolution, internalFormat, format, type);
}

void Texture::bindToTextureUnit(GLuint texture_unit_number) const
{
  glBindTextureUnit(texture_unit_number, texture);
}

void Texture::bindToImageUnit(GLuint image_unit_number, GLenum access) const
{
  glBindImageTexture(image_unit_number, this->texture, 0, GL_FALSE, 0, access,
                     this->internalFormat);
}

void Texture::release()
{
  if (texture) {
    spdlog::debug("[Texture] release texture {:x}", this->texture);

    glDeleteTextures(1, &this->texture);
    this->texture = 0;
  }
}