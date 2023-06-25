#pragma once

#include <assimp/scene.h>

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "assimp/material.h"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace ogls
{

class Model
{
   public:
    Model();
    Model(const std::filesystem::path& filepath);
    Model(const Model& other) = delete;
    Model(Model&& other);
    ~Model() = default;

    Model& operator=(const Model& other) = delete;
    Model& operator=(Model&& other);

    // does model have any meshes?
    operator bool() const;

    // load model with assimp
    void loadModel(const std::filesystem::path& filepath);

    uint32_t getNumberOfVertices() const;
    uint32_t getNumberOfFaces() const;
    uint32_t getNumberOfTextures() const;

    void draw(const Pipeline& pipeline) const;

   private:
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::vector<Texture> textures;

    using AssimpMaterialIndex = uint32_t;
    std::vector<AssimpMaterialIndex> loaded_materials;
    std::vector<std::filesystem::path> loaded_textures;

    void processAssimpNode(const aiNode* node, const aiScene* scene,
                           const std::filesystem::path& parentPath);

    Mesh processAssimpMesh(const aiMesh* mesh, const aiScene* scene,
                           const std::filesystem::path& parentPath);

    MaterialID loadMaterial(const aiScene* scene, AssimpMaterialIndex index,
                            const std::filesystem::path& parent_path);

    std::optional<MaterialID> getMaterialIndex(
        AssimpMaterialIndex assimp_material_id) const;

    Material loadMaterialFromAssimp(const aiMaterial* material,
                                    const std::filesystem::path& parent_path);

    std::optional<TextureID> loadTexture(
        const aiMaterial* material, const TextureType& type,
        const std::filesystem::path& parentPath);

    std::optional<TextureID> getTextureIndex(
        const std::filesystem::path& filepath) const;

    static std::vector<Vertex> getVerticesFromAssimp(const aiMesh* mesh);
    static std::vector<uint32_t> getIndicesFromAssimp(const aiMesh* mesh);

    static GLuint getTextureInternalFormat(const TextureType& type);

    static std::vector<uint8_t> loadImage(const std::filesystem::path& filepath,
                                          glm::vec2& resolution);

    static const std::map<TextureType, aiTextureType> assimp_texture_mapping;
};

}  // namespace ogls