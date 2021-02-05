#include "ogls/mesh.hpp"

namespace ogls {

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices, const Material& material,
           const std::vector<unsigned int>& indicesOfTextures)
    : vertices(vertices),
      indices(indices),
      material(material),
      indicesOfTextures(indicesOfTextures) {
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

  glBindVertexArray(0);
}

void Mesh::destroy() {
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
  vertices.clear();
  indices.clear();
  indicesOfTextures.clear();
}

void Mesh::draw(const Shader& shader,
                const std::vector<Texture>& textures) const {
  // set texture uniform
  std::size_t n_diffuse = 0;
  std::size_t n_specular = 0;
  for (std::size_t i = 0; i < indicesOfTextures.size(); ++i) {
    const Texture& texture = textures[indicesOfTextures[i]];
    const int textureUnitNumber = i;

    switch (texture.textureType) {
      case TextureType::Diffuse: {
        const std::string uniformName =
            "diffuseMaps[" + std::to_string(n_diffuse) + "]";
        shader.setUniformTexture(uniformName, texture.id, textureUnitNumber);
        n_diffuse++;
        break;
      }
      case TextureType::Specular: {
        const std::string uniformName =
            "specularMaps[" + std::to_string(n_specular) + "]";
        shader.setUniformTexture(uniformName, texture.id, textureUnitNumber);
        n_specular++;
        break;
      }
    }
  }

  // make kd zero if there are diffuseMaps
  glm::vec3 kd = material.kd;
  if (n_diffuse > 0) {
    kd = glm::vec3(0.0f);
  }
  // make ks zero if there are specularMaps
  glm::vec3 ks = material.ks;
  if (n_specular > 0) {
    ks = glm::vec3(0.0f);
  }

  // set material
  shader.setUniform("kd", kd);
  shader.setUniform("ks", ks);
  shader.setUniform("ka", material.ka);
  shader.setUniform("shininess", material.shininess);

  // draw mesh
  glBindVertexArray(VAO);
  shader.activate();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  shader.deactivate();
  glBindVertexArray(0);

  // reset texture uniform
  for (std::size_t i = 0; i < indicesOfTextures.size(); ++i) {
    shader.setUniformTexture("diffuseMaps[" + std::to_string(i) + "]", 0, 0);
    shader.setUniformTexture("specularMaps[" + std::to_string(i) + "]", 0, 0);
  }
}

}  // namespace ogls