#ifndef _OGLS_TEXTURE_H
#define _OGLS_TEXTURE_H
#include <string>

#include "glad/glad.h"
#include "stb_image.h"

namespace ogls {

enum class TextureType {
  Diffuse,
  Specular,
};

class Texture {
 public:
  std::string filepath;
  GLuint id;
  TextureType textureType;

  Texture();
  Texture(const std::string& filepath, const TextureType& textureType);

  // destroy texture object
  void destroy();

  void loadImage(const std::string& filepath) const;
};

}  // namespace ogls

#endif