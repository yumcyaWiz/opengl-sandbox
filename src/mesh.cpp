#include "mesh.hpp"

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
  if (material.diffuse_map) {
    shader.set_uniform_texture("material.diffuseMap",
                               textures[material.diffuse_map.value()].id, 1);
  }

  if (material.specular_map) {
    shader.set_uniform_texture("material.specularMap",
                               textures[material.specular_map.value()].id, 2);
  }

  if (material.ambient_map) {
    shader.set_uniform_texture("material.ambientMap",
                               textures[material.ambient_map.value()].id, 3);
  }

  if (material.emissive_map) {
    shader.set_uniform_texture("material.emissiveMap",
                               textures[material.emissive_map.value()].id, 4);
  }

  if (material.height_map) {
    shader.set_uniform_texture("material.heightMap",
                               textures[material.height_map.value()].id, 5);
    shader.set_uniform("material.hasHeightMap", true);
  } else {
    shader.set_uniform("material.hasHeightMap", false);
  }

  if (material.normal_map) {
    shader.set_uniform_texture("material.normalMap",
                               textures[material.normal_map.value()].id, 6);
    shader.set_uniform("material.hasNormalMap", true);
  } else {
    shader.set_uniform("material.hasNormalMap", false);
  }

  if (material.shininess_map) {
    shader.set_uniform_texture("material.shininessMap",
                               textures[material.shininess_map.value()].id, 7);
  }

  if (material.displacement_map) {
    shader.set_uniform_texture("material.displacementMap",
                               textures[material.displacement_map.value()].id,
                               8);
    shader.set_uniform("material.hasDisplacementMap", true);
  } else {
    shader.set_uniform("material.hasDisplacementMap", false);
  }

  if (material.light_map) {
    shader.set_uniform_texture("material.lightMap",
                               textures[material.light_map.value()].id, 9);
    shader.set_uniform("material.hasLightMap", true);
  } else {
    shader.set_uniform("material.hasLightMap", false);
  }

  // set material
  if (material.diffuse_map) {
    shader.set_uniform("material.kd", glm::vec3(0));
  } else {
    shader.set_uniform("material.kd", material.kd);
  }

  if (material.specular_map) {
    shader.set_uniform("material.ks", glm::vec3(0));
  } else {
    shader.set_uniform("material.ks", material.ks);
  }

  if (material.ambient_map) {
    shader.set_uniform("material.ka", glm::vec3(0));
  } else {
    shader.set_uniform("material.ka", material.ka);
  }

  if (material.emissive_map) {
    shader.set_uniform("material.ke", glm::vec3(0));
  } else {
    shader.set_uniform("material.ke", material.ke);
  }

  shader.set_uniform("material.shininess", material.shininess);

  // draw mesh
  glBindVertexArray(VAO);
  shader.activate();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  shader.deactivate();
  glBindVertexArray(0);

  // reset texture uniforms
  shader.set_uniform_texture("material.diffuseMap", 0, 0);
  shader.set_uniform_texture("material.specularMap", 0, 0);
  shader.set_uniform_texture("material.ambientMap", 0, 0);
  shader.set_uniform_texture("material.emissiveMap", 0, 0);
  shader.set_uniform_texture("material.heightMap", 0, 0);
  shader.set_uniform("material.hasHeightMap", false);
  shader.set_uniform_texture("material.normalMap", 0, 0);
  shader.set_uniform("material.hasNormalMap", false);
  shader.set_uniform_texture("material.shininessMap", 0, 0);
  shader.set_uniform_texture("material.displacementMap", 0, 0);
  shader.set_uniform("material.hasDisplacementMap", false);
  shader.set_uniform_texture("material.lightMap", 0, 0);
  shader.set_uniform("material.hasLightMap", false);
}

}  // namespace ogls