#include "quad.hpp"

using namespace ogls;

Quad::Quad()
{
    // setup VBO
    // position and texcoords
    const std::vector<GLfloat> vertices = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f};
    vertex_buffer.setData(vertices, GL_STATIC_DRAW);

    // setup EBO
    const std::vector<GLuint> indices = {0, 1, 2, 2, 3, 0};
    index_buffer.setData(indices, GL_STATIC_DRAW);

    // setup VAO
    VAO.bindVertexBuffer(vertex_buffer, 0, 0, 5 * sizeof(GLfloat));
    VAO.bindElementBuffer(index_buffer);

    // position
    VAO.activateVertexAttribution(0, 0, 3, GL_FLOAT, 0);

    // texcoords
    VAO.activateVertexAttribution(0, 1, 2, GL_FLOAT, 3 * sizeof(GLfloat));
}

void Quad::draw(const Pipeline& pipeline) const
{
    pipeline.activate();
    VAO.activate();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    VAO.deactivate();
    pipeline.deactivate();
}