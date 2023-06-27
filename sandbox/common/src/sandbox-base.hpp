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
    void initGL();

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

    // OpenGL debug message callback
    static void debugMessageCallback(GLenum source, GLenum type, GLuint id,
                                     GLenum severity, GLsizei length,
                                     GLchar const* message,
                                     void const* user_param);

    const std::map<GLenum, std::string> severity_map = {
        {GL_DEBUG_SEVERITY_HIGH, "HIGH"},
        {GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM"},
        {GL_DEBUG_SEVERITY_LOW, "LOW"},
        {GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"}};

    const std::map<GLenum, std::string> source_map = {
        {GL_DEBUG_SOURCE_API, "API"},
        {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "WINDOW SYSTEM"},
        {GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER COMPILER"},
        {GL_DEBUG_SOURCE_THIRD_PARTY, "THIRD PARTY"},
        {GL_DEBUG_SOURCE_APPLICATION, "APPLICATION"},
        {GL_DEBUG_SOURCE_OTHER, "OTHER"}};

    const std::map<GLenum, std::string> type_map = {
        {GL_DEBUG_TYPE_ERROR, "ERROR"},
        {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR"},
        {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED_BEHAVIOR"},
        {GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY"},
        {GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE"},
        {GL_DEBUG_TYPE_MARKER, "MARKER"},
        {GL_DEBUG_TYPE_OTHER, "OTHER"}};
};

}  // namespace sandbox