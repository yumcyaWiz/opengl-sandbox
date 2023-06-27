#include "sandbox-base.hpp"

#include <stdexcept>

namespace sandbox
{

SandboxBase::SandboxBase(uint32_t width, uint32_t height)
    : width{width}, height{height}
{
#ifdef NDEBUG
    spdlog::set_level(spdlog::level::warn);
#else
    spdlog::set_level(spdlog::level::debug);
#endif

    initGlfw();
    initGlad();
    initGL();
    initImGui();

    scene.init();
}

SandboxBase::~SandboxBase() { release(); }

void SandboxBase::initGlfw()
{
    if (!glfwInit()) { throw std::runtime_error("failed to initialize GLFW"); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // required for Mac
    // glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, "model-viewer", nullptr, nullptr);
    if (!window) { throw std::runtime_error("failed to create window"); }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackStatic);
}

void SandboxBase::initGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("failed to initialize glad");
    }
}

void SandboxBase::initImGui()
{
    // initialize imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();

    // set imgui style
    ImGui::StyleColorsDark();

    // initialize imgui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

void SandboxBase::initGL()
{
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugMessageCallback, nullptr);
#endif
}

void SandboxBase::framebufferSizeCallback(GLFWwindow *window, int width,
                                          int height)
{
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
}

void SandboxBase::framebufferSizeCallbackStatic(GLFWwindow *window, int width,
                                                int height)
{
    SandboxBase *instance =
        static_cast<SandboxBase *>(glfwGetWindowUserPointer(window));
    instance->framebufferSizeCallback(window, width, height);
}

void SandboxBase::debugMessageCallback(GLenum source, GLenum type, GLuint id,
                                       GLenum severity, GLsizei length,
                                       GLchar const *message,
                                       void const *user_param)
{
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            spdlog::critical("[gl] {}", message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            spdlog::error("[gl] {}", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            spdlog::warn("[gl] {}", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            spdlog::info("[gl] {}", message);
            break;
    }
}

void SandboxBase::release()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void SandboxBase::run()
{
    beforeRender();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // start imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        runImGui();

        handleInput();

        render();

        // render imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

}  // namespace sandbox