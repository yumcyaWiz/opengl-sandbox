#pragma once
#include <filesystem>

#include "ogls.hpp"

class DepthMap
{
 private:
  int width;
  int height;

  ogls::Texture texture;
  ogls::FrameBuffer fbo;
  ogls::Pipeline pipeline;

 public:
  DepthMap(int width, int height)
      : width(width), height(height), fbo({GL_DEPTH_ATTACHMENT})
  {
    pipeline.loadVertexShader(std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
                              "shaders/make-depthmap.vert");
    pipeline.loadFragmentShader(
        std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
        "shaders/make-depthmap.frag");

    texture = ogls::Texture::TextureBuilder({width, height})
                  .setInternalFormat(GL_DEPTH_COMPONENT16)
                  .setFormat(GL_DEPTH_COMPONENT)
                  .setType(GL_FLOAT)
                  .setWrapS(GL_CLAMP_TO_EDGE)
                  .setWrapT(GL_CLAMP_TO_EDGE)
                  .setDepthCompareMode(true)
                  .build();

    fbo.bindTexture(texture, 0);
    fbo.setDrawBuffer(GL_NONE);
  }

  const ogls::Texture& getTextureRef() const { return texture; }

  void setResolution(int width, int height)
  {
    this->width = width;
    this->height = height;

    texture = ogls::Texture::TextureBuilder({width, height})
                  .setInternalFormat(GL_DEPTH_COMPONENT16)
                  .setFormat(GL_DEPTH_COMPONENT)
                  .setType(GL_FLOAT)
                  .setWrapS(GL_CLAMP_TO_EDGE)
                  .setWrapT(GL_CLAMP_TO_EDGE)
                  .setDepthCompareMode(true)
                  .build();

    fbo.bindTexture(texture, 0);
  }

  void setLightSpaceMatrix(const glm::mat4& lightSpaceMatrix)
  {
    pipeline.setUniform("lightSpaceMatrix", lightSpaceMatrix);
  }

  void draw(const ogls::Scene& scene) const
  {
    // render to depth map
    glViewport(0, 0, width, height);
    fbo.activate();
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);  // prevent peter panning
    scene.draw(pipeline);
    glCullFace(GL_BACK);
    fbo.deactivate();
  }
};