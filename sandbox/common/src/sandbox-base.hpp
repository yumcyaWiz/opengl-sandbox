#pragma once

#include "glad/glad.h"
//
#include "GLFW/glfw3.h"
#include "imgui.h"
//
#include "ogls.hpp"

namespace sandbox
{

class SandboxBase
{
 public:
  SandboxBase(uint32_t width, uint32_t height);
  ~SandboxBase();

  void run();

  void initGlfw();
  void initGlad();
  void initImGui();

  virtual void beforeRender() = 0;
  virtual void runImGui() const = 0;
  virtual void handleInput() const = 0;
  virtual void render() const = 0;

  virtual void release();
  virtual void framebufferSizeCallback(GLFWwindow* window, int width,
                                       int height);

 protected:
  uint32_t width;
  uint32_t height;
  GLFWwindow* window;
  ImGuiIO* io;

  ogls::Camera camera;
  ogls::Scene scene;

 private:
  static void framebufferSizeCallbackStatic(GLFWwindow* window, int width,
                                            int height);
};

}  // namespace sandbox