#include "texture.hpp"

using namespace ogls;

Texture::Texture() {}

Texture::Texture(const TextureBuilder& builder)
{
    resolution = builder.resolution;
    internalFormat = builder.internalFormat;
    format = builder.format;
    type = builder.type;
    wrap_s = builder.wrap_s;
    wrap_t = builder.wrap_t;
    mag_filter = builder.mag_filter;
    min_filter = builder.min_filter;
    generate_mipmap = builder.generate_mipmap;
    depth_compare_mode = builder.depth_compare_mode;

    createTexture();
    setImage(builder.image);

    spdlog::debug("[Texture] texture {:x} created", this->texture);
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

void Texture::createTexture()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, wrap_s);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, wrap_t);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, min_filter);
    if (depth_compare_mode) {
        glTextureParameteri(texture, GL_TEXTURE_COMPARE_MODE,
                            GL_COMPARE_REF_TO_TEXTURE);
    }
}

void Texture::setImage(const void* image) const
{
    glTextureStorage2D(texture, 1, internalFormat, resolution.x, resolution.y);
    glTextureSubImage2D(texture, 0, 0, 0, resolution.x, resolution.y, format,
                        type, image);
    if (generate_mipmap) { glGenerateTextureMipmap(texture); }
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