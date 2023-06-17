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
bool SHOW_NORMAL = false;
bool USE_NORMAL_MAP = false;

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
    CAMERA->look_around(io.MouseDelta.x, io.MouseDelta.y);
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
      glfwCreateWindow(WIDTH, HEIGHT, "normal-mapping", nullptr, nullptr);
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
  scene.add_point_light({glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f});
  scene.set_point_light_index(0);

  // setup shader
  const Shader vertex_shader = Shader::create_vertex_shader(
      std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) / "shaders/shader.vert");
  const Shader fragment_shader = Shader::create_fragment_shader(
      std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) / "shaders/shader.frag");

  const Pipeline pipeline;
  pipeline.attachVertexShader(vertex_shader);
  pipeline.attachFragmentShader(fragment_shader);

  Model *model = nullptr;

  // app loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // imgui
    ImGui::Begin("UI");
    {
      static char modelPath[100] = {"assets/normal-mapping-test/test.obj"};
      ImGui::InputText("Model", modelPath, 100);
      if (ImGui::Button("Load Model")) {
        if (model) { delete model; }
        model = new Model{std::string(CMAKE_SOURCE_DIR) + "/" + modelPath};
        scene.set_model(model);
      }

      ImGui::Separator();

      ImGui::InputFloat("FOV", &CAMERA->fov);
      ImGui::InputFloat("Movement Speed", &CAMERA->movement_speed);
      ImGui::InputFloat("Look Around Speed", &CAMERA->look_around_speed);

      if (ImGui::Button("Reset Camera")) { CAMERA->reset(); }

      ImGui::Separator();

      ImGui::Checkbox("Normal Mapping", &USE_NORMAL_MAP);
      ImGui::Checkbox("Show Normal", &SHOW_NORMAL);
    }
    ImGui::End();

    handleInput(window, io);

    // set uniform variables
    vertex_shader.setUniform("view", CAMERA->compute_view_matrix());
    vertex_shader.setUniform("projection",
                             CAMERA->compute_projection_matrix(WIDTH, HEIGHT));
    fragment_shader.setUniform("camPos", CAMERA->cam_pos);
    fragment_shader.setUniform("useNormalMap", USE_NORMAL_MAP);
    fragment_shader.setUniform("showNormal", SHOW_NORMAL);

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