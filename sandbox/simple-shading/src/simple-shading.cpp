#include <filesystem>
#include <iostream>
#include <memory>

#include "glad/glad.h"
//
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//
#include "ogls/camera.hpp"
#include "ogls/model.hpp"
#include "ogls/scene.hpp"
#include "ogls/shader.hpp"

using namespace ogls;

// globals
std::unique_ptr<Camera> camera;
int width = 1600;
int height = 900;

void handleInput(GLFWwindow* window, const ImGuiIO& io) {
  // close application
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  // camera movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera->move(CameraMovement::FORWARD, io.DeltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera->move(CameraMovement::LEFT, io.DeltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera->move(CameraMovement::BACKWARD, io.DeltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera->move(CameraMovement::RIGHT, io.DeltaTime);
  }

  // camera look around
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    camera->lookAround(io.MouseDelta.x, io.MouseDelta.y);
  }
}

void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int _width,
                             int _height) {
  width = _width;
  height = _height;
  glViewport(0, 0, width, height);
}

int main() {
  // initialize glfw
  if (!glfwInit()) {
    std::cerr << "failed to initialize GLFW" << std::endl;
    return EXIT_FAILURE;
  }

  // setup window and OpenGL context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // required for Mac
  glfwWindowHint(GLFW_SAMPLES, 4);                      // 4x MSAA
  GLFWwindow* window =
      glfwCreateWindow(width, height, "simple-shading", nullptr, nullptr);
  if (!window) {
    std::cerr << "failed to create window" << std::endl;
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  // initialize glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "failed to initialize glad" << std::endl;
    return EXIT_FAILURE;
  }

  // initialize imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
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
  camera = std::make_unique<Camera>();

  // setup scene
  Scene scene;
  scene.addPointLight({glm::vec3(10000.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f});
  scene.setPointLightIndex(0);

  // setup shader
  Shader shader;
  shader.setVertexShader(std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
                         "shaders/shader.vert");
  shader.setFragmentShader(std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
                           "shaders/shader.frag");
  shader.linkShader();

  // app loop
  float t = 0.0f;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // imgui
    ImGui::Begin("viewer");

    static char modelPath[100] = {"assets/sponza/sponza.obj"};
    ImGui::InputText("Model", modelPath, 100);
    if (ImGui::Button("Load Model")) {
      scene.setModel({std::string(CMAKE_SOURCE_DIR) + "/" + modelPath});
    }

    ImGui::InputFloat("FOV", &camera->fov);
    ImGui::InputFloat("Movement Speed", &camera->movementSpeed);
    ImGui::InputFloat("Look Around Speed", &camera->lookAroundSpeed);

    if (ImGui::Button("Reset Camera")) {
      camera->reset();
    }

    ImGui::End();

    handleInput(window, io);

    // update light position
    t += io.DeltaTime;
    scene.pointLights[0].position =
        glm::vec3(100.0f * std::cos(t), 100.0f, 100.0f * std::sin(t));

    // set uniform variables
    shader.setUniform("view", camera->computeViewMatrix());
    shader.setUniform("projection",
                      camera->computeProjectionMatrix(width, height));
    shader.setUniform("camPos", camera->camPos);

    // render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.draw(shader);

    // render imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  // exit
  shader.destroy();
  scene.destroy();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}