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

    // implement setup actions before rendering loop
    virtual void beforeRender() = 0;

    // implement imgui actions inside rendering loop
    virtual void runImGui() = 0;

    // implement input handling inside rendering loop
    virtual void handleInput() = 0;

    // implement rendering actions inside rendering loop
    virtual void render() = 0;

    // implement resource release actions before closing the app
    virtual void release();

    // implement framebuffer size callback
    virtual void framebufferSizeCallback(GLFWwindow* window, int width,
                                         int height);

    static void framebufferSizeCallbackStatic(GLFWwindow* window, int width,
                                              int height);
};

}  // namespace sandbox