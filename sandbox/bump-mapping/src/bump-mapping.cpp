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
#include "camera.hpp"
#include "model.hpp"
#include "scene.hpp"
#include "shader.hpp"

using namespace ogls;

// globals
std::unique_ptr<Camera> CAMERA;
int WIDTH = 1600;
int HEIGHT = 900;
float LINE_LENGTH = 1;
bool USE_HEIGHT_MAP = false;
int HEIGHT_MAP_METHOD = 1;
float HEIGHT_MAP_SCALE = 0.01f;

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
  glViewport(0, 0, WIDTH, HEIGHT);
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
  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "bump-mapping", nullptr, nullptr);
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
  ImGui_ImplOpenGL3_Init("#version 460 core");

  // enable depth test
  glEnable(GL_DEPTH_TEST);
  // enable MSAA
  glEnable(GL_MULTISAMPLE);

  // initialize camera
  CAMERA = std::make_unique<Camera>();

  // setup scene
  Scene scene;
  scene.setPointLight({glm::vec3(10000), glm::vec3(0, 100, 0), 0});

  // setup shader
  const Shader vertex_shader = Shader::createVertexShader(
      std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) / "shaders/shader.vert");
  const Shader fragment_shader = Shader::createFragmentShader(
      std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) / "shaders/shader.frag");

  const Pipeline pipeline;
  pipeline.attachVertexShader(vertex_shader);
  pipeline.attachFragmentShader(fragment_shader);

  Model *model = nullptr;

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
    {
      static char modelPath[100] = {"assets/sponza/sponza.obj"};
      ImGui::InputText("Model", modelPath, 100);
      if (ImGui::Button("Load Model")) {
        if (model) { delete model; }
        model = new Model(std::string(CMAKE_SOURCE_DIR) + "/" + modelPath);
        scene.setModel(model);
      }

      ImGui::Separator();

      ImGui::InputFloat("FOV", &CAMERA->fov);
      ImGui::InputFloat("Movement Speed", &CAMERA->movement_speed);
      ImGui::InputFloat("Look Around Speed", &CAMERA->look_around_speed);

      if (ImGui::Button("Reset Camera")) { CAMERA->reset(); }

      ImGui::Separator();

      ImGui::Checkbox("Use Height Map", &USE_HEIGHT_MAP);
      ImGui::Combo("Height Map Method", &HEIGHT_MAP_METHOD,
                   "Strict\0Usual\0\0");
      ImGui::InputFloat("Height Map Scale", &HEIGHT_MAP_SCALE);
    }
    ImGui::End();

    handleInput(window, io);

    // set light position
    t += io.DeltaTime;
    scene.setPointLight({glm::vec3(1.0f),
                         CAMERA->cam_pos + 200.0f * CAMERA->cam_forward, 0.0f});

    // set uniform variables
    const glm::mat4 view = CAMERA->computeViewMatrix();
    const glm::mat4 projection = CAMERA->computeProjectionMatrix(WIDTH, HEIGHT);
    vertex_shader.setUniform("view", view);
    vertex_shader.setUniform("projection", projection);
    fragment_shader.setUniform("camPos", CAMERA->cam_pos);
    fragment_shader.setUniform("useHeightMap", USE_HEIGHT_MAP);
    fragment_shader.setUniform("heightMapMethod", HEIGHT_MAP_METHOD);
    fragment_shader.setUniform("heightMapScale", HEIGHT_MAP_SCALE);

    // render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.draw(pipeline, fragment_shader);

    // render imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  // exit
  delete model;

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}