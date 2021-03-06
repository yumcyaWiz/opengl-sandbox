#ifndef _OGLS_QUAD_H
#define _OGLS_QUAD_H

#include "glad/glad.h"
#include "shader.hpp"

namespace ogls {

class Quad {
 private:
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;

 public:
  Quad();

  void destroy();

  void draw(const Shader& shader) const;
};

}  // namespace ogls

#endif