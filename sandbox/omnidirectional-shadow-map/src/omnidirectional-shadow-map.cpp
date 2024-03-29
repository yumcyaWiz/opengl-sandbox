#include <filesystem>
#include <iostream>
#include <memory>

#include "glad/glad.h"
//
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//
#include "ogls.hpp"
//
#include "omnidirectional-shadow-map.h"

using namespace ogls;

// globals
std::unique_ptr<Camera> CAMERA;
int WIDTH = 1600;
int HEIGHT = 900;
int SHADOW_MAP_RES = 1024;
float SHADOW_BIAS = 10.0f;

void handleInput(GLFWwindow *window, const ImGuiIO &io)
{
    // close application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        CAMERA->move(CameraMovement::FORWARD, io.DeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        CAMERA->move(CameraMovement::LEFT, io.DeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        CAMERA->move(CameraMovement::BACKWARD, io.DeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        CAMERA->move(CameraMovement::RIGHT, io.DeltaTime);
    }

    // camera look around
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        CAMERA->lookAround(io.MouseDelta.x, io.MouseDelta.y);
    }
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int _width,
                               int _height)
{
    WIDTH = _width;
    HEIGHT = _height;
}

int main()
{
    // initialize glfw
    if (!glfwInit()) {
        std::cerr << "failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    // setup window and OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // required for Mac
    glfwWindowHint(GLFW_SAMPLES, 4);                      // 4x MSAA
    GLFWwindow *window = glfwCreateWindow(
        WIDTH, HEIGHT, "omnidirectional-shadow-map", nullptr, nullptr);
    if (!window) {
        std::cerr << "failed to create window" << std::endl;
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "failed to initialize glad" << std::endl;
        return EXIT_FAILURE;
    }

    // initialize imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // set imgui style
    ImGui::StyleColorsDark();

    // initialize imgui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // enable depth test
    glEnable(GL_DEPTH_TEST);
    // enable MSAA
    glEnable(GL_MULTISAMPLE);

    // initialize camera
    CAMERA = std::make_unique<Camera>();

    // setup scene
    Scene scene;
    scene.setPointLight({glm::vec3(10000.0f), glm::vec3(0, 100.0f, 0), 0.0f});

    // setup shader
    Pipeline pipeline;
    pipeline.loadVertexShader(std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
                              "shaders/shader.vert");
    pipeline.loadFragmentShader(
        std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
        "shaders/shader.frag");

    OmnidirectionalShadowMap shadowMap(SHADOW_MAP_RES, SHADOW_MAP_RES);

    // app loop
    float t = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // start imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // imgui
        ImGui::Begin("UI");

        static char modelPath[100] = {"assets/sponza/sponza.obj"};
        ImGui::InputText("Model", modelPath, 100);
        if (ImGui::Button("Load Model")) {
            scene.setModel({std::string(CMAKE_SOURCE_DIR) + "/" + modelPath});
        }

        ImGui::Separator();

        ImGui::InputFloat("FOV", &CAMERA->fov);
        ImGui::InputFloat("Movement Speed", &CAMERA->movement_speed);
        ImGui::InputFloat("Look Around Speed", &CAMERA->look_around_speed);

        if (ImGui::Button("Reset Camera")) { CAMERA->reset(); }

        ImGui::Separator();

        if (ImGui::InputInt("Shadow Map Resolution", &SHADOW_MAP_RES)) {
            shadowMap.setResolution(SHADOW_MAP_RES, SHADOW_MAP_RES);
        }
        ImGui::InputFloat("Shadow Bias", &SHADOW_BIAS);

        ImGui::End();

        handleInput(window, io);

        // move point light
        t += io.DeltaTime;

        glm::vec3 point_light_pos = glm::vec3(
            100.0f * std::cos(t), 300.0f * (std::sin(0.1f * t) + 1.0f) + 100.0f,
            100.0f * std::sin(t));
        scene.setPointLight({glm::vec3(10000.0f), point_light_pos, 0.0f});

        // make depth map
        shadowMap.setLightPosition(point_light_pos);
        shadowMap.draw(scene);

        // render scene with shadow mapping
        // set uniforms
        pipeline.setUniform("viewProjection",
                            CAMERA->computeViewProjectionMatrix(WIDTH, HEIGHT));
        pipeline.setUniform("camPos", CAMERA->cam_pos);
        pipeline.setUniform("shadowBias", SHADOW_BIAS);
        // TODO: set texture unit number appropriately
        glBindTextureUnit(10, shadowMap.cubemap);
        pipeline.setUniform("shadowMap", 10);
        pipeline.setUniform("zFar", shadowMap.zFar);

        // render
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.draw(pipeline);

        // render imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // exit
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}