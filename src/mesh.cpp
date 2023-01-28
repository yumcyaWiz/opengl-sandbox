#include "ogls/mesh.hpp"

namespace ogls
{

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices, const Material& material)
    : vertices{vertices}, indices{indices}, material{material}
{
  // setup VBO, EBO, VAO
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  // EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  // position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(0));
  // normal
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, normal)));

  // texcoords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, texcoords)));

  // tangent
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, tangent)));

  // dndu
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, dndu)));

  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, dndv)));

  glBindVertexArray(0);
}

void Mesh::destroy()
{
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
  vertices.clear();
  indices.clear();
}

void Mesh::draw(const Shader& shader,
                const std::vector<Texture>& textures) const
{
  // set texture uniform
  if (material.diffuseMap) {
    shader.setUniformTexture("material.diffuseMap",
                             textures[material.diffuseMap.value()].id, 1);
  }

  if (material.specularMap) {
    shader.setUniformTexture("material.specularMap",
                             textures[material.specularMap.value()].id, 2);
  }

  if (material.ambientMap) {
    shader.setUniformTexture("material.ambientMap",
                             textures[material.ambientMap.value()].id, 3);
  }

  if (material.emissiveMap) {
    shader.setUniformTexture("material.emissiveMap",
                             textures[material.emissiveMap.value()].id, 4);
  }

  if (material.heightMap) {
    shader.setUniformTexture("material.heightMap",
                             textures[material.heightMap.value()].id, 5);
    shader.setUniform("material.hasHeightMap", true);
  } else {
    shader.setUniform("material.hasHeightMap", false);
  }

  if (material.normalMap) {
    shader.setUniformTexture("material.normalMap",
                             textures[material.normalMap.value()].id, 6);
    shader.setUniform("material.hasNormalMap", true);
  } else {
    shader.setUniform("material.hasNormalMap", false);
  }

  if (material.shininessMap) {
    shader.setUniformTexture("material.shininessMap",
                             textures[material.shininessMap.value()].id, 7);
  }

  if (material.displacementMap) {
    shader.setUniformTexture("material.displacementMap",
                             textures[material.displacementMap.value()].id, 8);
    shader.setUniform("material.hasDisplacementMap", true);
  } else {
    shader.setUniform("material.hasDisplacementMap", false);
  }

  if (material.lightMap) {
    shader.setUniformTexture("material.lightMap",
                             textures[material.lightMap.value()].id, 9);
    shader.setUniform("material.hasLightMap", true);
  } else {
    shader.setUniform("material.hasLightMap", false);
  }

  // set material
  if (material.diffuseMap) {
    shader.setUniform("material.kd", glm::vec3(0));
  } else {
    shader.setUniform("material.kd", material.kd);
  }

  if (material.specularMap) {
    shader.setUniform("material.ks", glm::vec3(0));
  } else {
    shader.setUniform("material.ks", material.ks);
  }

  if (material.ambientMap) {
    shader.setUniform("material.ka", glm::vec3(0));
  } else {
    shader.setUniform("material.ka", material.ka);
  }

  if (material.emissiveMap) {
    shader.setUniform("material.ke", glm::vec3(0));
  } else {
    shader.setUniform("material.ke", material.ke);
  }

  shader.setUniform("material.shininess", material.shininess);

  // draw mesh
  glBindVertexArray(VAO);
  shader.activate();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  shader.deactivate();
  glBindVertexArray(0);

  // reset texture uniforms
  shader.setUniformTexture("material.diffuseMap", 0, 0);
  shader.setUniformTexture("material.specularMap", 0, 0);
  shader.setUniformTexture("material.ambientMap", 0, 0);
  shader.setUniformTexture("material.emissiveMap", 0, 0);
  shader.setUniformTexture("material.heightMap", 0, 0);
  shader.setUniform("material.hasHeightMap", false);
  shader.setUniformTexture("material.normalMap", 0, 0);
  shader.setUniform("material.hasNormalMap", false);
  shader.setUniformTexture("material.shininessMap", 0, 0);
  shader.setUniformTexture("material.displacementMap", 0, 0);
  shader.setUniform("material.hasDisplacementMap", false);
  shader.setUniformTexture("material.lightMap", 0, 0);
  shader.setUniform("material.hasLightMap", false);
}

}  // namespace ogls