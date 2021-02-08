#ifndef _OGLS_TEXTURE_H
#define _OGLS_TEXTURE_H
#include <filesystem>
#include <iostream>

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

inline std::ostream& operator<<(std::ostream& stream, const TextureType& type) {
  switch (type) {
    case TextureType::Diffuse:
      stream << "Diffuse";
      break;
    case TextureType::Specular:
      stream << "Specular";
      break;
    case TextureType::Ambient:
      stream << "Ambient";
      break;
    case TextureType::Emissive:
      stream << "Emissive";
      break;
    case TextureType::Height:
      stream << "Height";
      break;
    case TextureType::Normal:
      stream << "Normal";
      break;
    case TextureType::Shininess:
      stream << "Normal";
      break;
    case TextureType::Displacement:
      stream << "Displacement";
      break;
    case TextureType::Light:
      stream << "Light";
      break;
  }

  return stream;
}

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

  void loadImage(const std::filesystem::path& filepath) const;
};

}  // namespace ogls

#endif