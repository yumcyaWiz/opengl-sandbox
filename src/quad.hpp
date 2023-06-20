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
  Buffer vertex_buffer;
  Buffer index_buffer;

 public:
  Quad();

  void draw(const Pipeline& pipeline) const;
};

}  // namespace ogls