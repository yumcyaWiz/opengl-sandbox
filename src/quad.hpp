#pragma once
#include <vector>

#include "glad/glad.h"
//
#include "buffer.hpp"
#include "shader.hpp"
#include "vertex-array-object.hpp"

namespace ogls
{

class Quad
{
 private:
  VertexArrayObject VAO;
  Buffer VBO;
  Buffer EBO;

 public:
  Quad();

  void draw(const Pipeline& pipeline) const;
};

}  // namespace ogls