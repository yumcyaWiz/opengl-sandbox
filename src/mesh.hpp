#pragma once

#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
//
#include "buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex-array-object.hpp"

namespace ogls
{

using MaterialID = uint32_t;
using TextureID = uint32_t;

struct Material {
    // diffuse color
    glm::vec3 kd = glm::vec3(0.0f);
    // specular color
    glm::vec3 ks = glm::vec3(0.0f);
    // ambient color
    glm::vec3 ka = glm::vec3(0.0f);
    // emissive color
    glm::vec3 ke = glm::vec3(0.0f);
    float shininess = 0.0f;

    // index of diffuse map texture
    std::optional<TextureID> diffuse_map = std::nullopt;
    // index of specular map texture
    std::optional<TextureID> specular_map = std::nullopt;
    // index of ambient map texture
    std::optional<TextureID> ambient_map = std::nullopt;
    // index of emissive map texture
    std::optional<TextureID> emissive_map = std::nullopt;
    // index of height map texture
    std::optional<TextureID> height_map = std::nullopt;
    // index of normal map texture
    std::optional<TextureID> normal_map = std::nullopt;
    // index of shininess map texture
    std::optional<TextureID> shininess_map = std::nullopt;
    // index of displacement map texture
    std::optional<TextureID> displacement_map = std::nullopt;
    // index of light map texture
    std::optional<TextureID> light_map = std::nullopt;

    Material() {}
};

struct Vertex {
    glm::vec3 position = glm::vec3(0.0f);   // vertex position
    glm::vec3 normal = glm::vec3(0.0f);     // vertex normal
    glm::vec2 texcoords = glm::vec3(0.0f);  // texture coordinates
    glm::vec3 tangent = glm::vec3(0.0f);    // tangent vector(dp/du)
    glm::vec3 dndu = glm::vec3(0.0f);  // differential of normal by texcoords
    glm::vec3 dndv = glm::vec3(0.0f);  // differential of normal by texcoords

    Vertex() {}
};

// TODO: maybe this class should be data class and all the methods should be
// moved to Model class
class Mesh

{
   public:
    Mesh();
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indices, MaterialID material_index);
    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other);
    ~Mesh() = default;

    Mesh& operator=(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other);

    // TODO: should be placed in Model class
    void draw(const Pipeline& pipeline, const Material& material,
              const std::vector<Texture>& textures) const;

    uint32_t getNumberOfVertices() const;
    uint32_t getNumberOfFaces() const;
    MaterialID getMaterialID() const;

   private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    // TODO: remove this field, this is only used in Model class
    MaterialID material_id;

    VertexArrayObject vao;
    Buffer vertex_buffer;
    Buffer index_buffer;
};

}  // namespace ogls