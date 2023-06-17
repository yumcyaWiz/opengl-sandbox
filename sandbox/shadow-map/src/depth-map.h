#pragma once
#include <filesystem>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
//
#include "scene.hpp"
#include "shader.hpp"

using namespace ogls;

class DepthMap
{
 public:
  int width;
  int height;
  GLuint FBO;
  GLuint texture;
  Shader shader;

  DepthMap(int width, int height) : width(width), height(height)
  {
    shader.load_vertex_shader(std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
                              "shaders/make-depthmap.vert");
    shader.load_fragment_shader(
        std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
        "shaders/make-depthmap.frag");
    shader.link_shader();

    // setup depth map FBO
    glGenFramebuffers(1, &FBO);

    // setup depth map texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    constexpr float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // attach depth map texture to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           texture, 0);
    glDrawBuffer(GL_NONE);  // disable draw buffer
    glReadBuffer(GL_NONE);  // disable read buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void destroy()
  {
    shader.destroy();
    glDeleteTextures(1, &texture);
    glDeleteFramebuffers(1, &FBO);
  }

  void setResolution(int width, int height)
  {
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void setLightSpaceMatrix(const glm::mat4& lightSpaceMatrix)
  {
    shader.set_uniform("lightSpaceMatrix", lightSpaceMatrix);
  }

  void draw(const Scene& scene) const
  {
    // render to depth map
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);  // prevent peter panning
    scene.draw(shader);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
};