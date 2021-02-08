#ifndef _OGLS_TEXTURE_H
#define _OGLS_TEXTURE_H
#include <filesystem>
#include <iostream>
#include <string>

#include "glad/glad.h"

namespace ogls {

enum class TextureType {
  Diffuse,
  Specular,
  Ambient,
  Emissive,
  Height,
  Normal,
  Shininess,
  Displacement,
  Light,
};

class Texture {
 public:
  std::filesystem::path filepath;
  GLuint id;
  TextureType textureType;

  Texture();
  Texture(const std::filesystem::path& filepath,
          const TextureType& textureType);

  // destroy texture object
  void destroy();

  // return texture type string
  std::string typeName() const;

  void loadImage(const std::filesystem::path& filepath) const;
};

}  // namespace ogls

#endif