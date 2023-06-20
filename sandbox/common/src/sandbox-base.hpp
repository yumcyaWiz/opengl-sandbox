#pragma once

#include "glad/glad.h"
//
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//
#include "ogls.hpp"

namespace sandbox
{

class SandboxBase
{
 public:
  SandboxBase(uint32_t width, uint32_t height);
  virtual ~SandboxBase();

  void run();

 protected:
  uint32_t width;
  uint32_t height;
  GLFWwindow* window;
  ImGuiIO* io;

  ogls::Camera camera;
  ogls::Scene scene;

 private:
  void initGlfw();
  void initGlad();
  void initImGui();

  virtual void beforeRender() = 0;
  virtual void runImGui() = 0;
  virtual void handleInput() = 0;
  virtual void render() const = 0;

  virtual void release();
  virtual void framebufferSizeCallback(GLFWwindow* window, int width,
                                       int height);

  static void framebufferSizeCallbackStatic(GLFWwindow* window, int width,
                                            int height);
};

}  // namespace sandbox