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
#include "ogls/camera.hpp"
#include "ogls/model.hpp"
#include "ogls/quad.hpp"
#include "ogls/scene.hpp"
#include "ogls/shader.hpp"
//
#include "depth-map.h"

using namespace ogls;

// globals
std::unique_ptr<Camera> CAMERA;
int WIDTH = 1600;
int HEIGHT = 900;
int DEPTH_MAP_RES = 1024;
float DEPTH_BIAS = 0.005f;
float DEPTH_MAP_NEAR = 100.0f;
float DEPTH_MAP_FAR = 10000.0f;
float DEPTH_MAP_SIZE = 2000.0f;
float LIGHT_DISTANCE = 2000.0f;

void handleInput(GLFWwindow* window, const ImGuiIO& io) {
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

void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int _width,
                             int _height) {
  WIDTH = _width;
  HEIGHT = _height;
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
      glfwCreateWindow(WIDTH, HEIGHT, "shadow-map", nullptr, nullptr);
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
  CAMERA = std::make_unique<Camera>();

  // setup scene
  Scene scene;
  scene.setDirectionalLight(
      {glm::vec3(1.0f), glm::normalize(glm::vec3(0.5f, 1.0f, 0.5f))});

  // quad for showing depth map
  Quad quad;

  // setup shader
  Shader showDepthMap{
      std::string(CMAKE_CURRENT_SOURCE_DIR) + "/shaders/show-depthmap.vert",
      std::string(CMAKE_CURRENT_SOURCE_DIR) + "/shaders/show-depthmap.frag"};

  Shader shader{std::string(CMAKE_CURRENT_SOURCE_DIR) + "/shaders/shader.vert",
                std::string(CMAKE_CURRENT_SOURCE_DIR) + "/shaders/shader.frag"};

  DepthMap depthMap(1024, 1024);

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
    ImGui::InputFloat("Movement Speed", &CAMERA->movementSpeed);
    ImGui::InputFloat("Look Around Speed", &CAMERA->lookAroundSpeed);

    if (ImGui::Button("Reset Camera")) {
      CAMERA->reset();
    }

    ImGui::Separator();

    if (ImGui::InputInt("Depth Map Resolution", &DEPTH_MAP_RES)) {
      depthMap.setResolution(DEPTH_MAP_RES, DEPTH_MAP_RES);
    }
    ImGui::InputFloat("Depth Bias", &DEPTH_BIAS);
    ImGui::InputFloat("Depth Map Size", &DEPTH_MAP_SIZE);
    ImGui::InputFloat("Directional Light Distance", &LIGHT_DISTANCE);
    ImGui::InputFloat("Depth Map zNear", &DEPTH_MAP_NEAR);
    ImGui::InputFloat("Depth Map zFar", &DEPTH_MAP_FAR);

    ImGui::End();

    handleInput(window, io);

    // update light direction
    t += 0.1f * io.DeltaTime;
    scene.directionalLight.direction =
        glm::normalize(glm::vec3(0.5f * glm::cos(t), 1.0f, 0.5f * std::sin(t)));

    // set view, projection matrix for making depth map
    const glm::mat4 lightView =
        glm::lookAt(LIGHT_DISTANCE * scene.directionalLight.direction,
                    glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 lightProjection =
        glm::ortho(-DEPTH_MAP_SIZE, DEPTH_MAP_SIZE, -DEPTH_MAP_SIZE,
                   DEPTH_MAP_SIZE, DEPTH_MAP_NEAR, DEPTH_MAP_FAR);
    const glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    depthMap.setLightSpaceMatrix(lightSpaceMatrix);

    // make depth map
    depthMap.draw(scene);

    // render scene with shadow mapping
    // set uniforms
    shader.setUniform("viewProjection",
                      CAMERA->computeViewProjectionMatrix(WIDTH, HEIGHT));
    shader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
    shader.setUniform("camPos", CAMERA->camPos);
    // TODO: set texture unit number appropriately
    shader.setUniformTexture("depthMap", depthMap.texture, 10);
    shader.setUniform("depthBias", DEPTH_BIAS);

    // render
    glViewport(0, 0, WIDTH, HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.draw(shader);

    // show depth map
    /*
    glViewport(0, 0, WIDTH, HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    showDepthMap.setUniformTexture("depthMap", depthMap, 0);
    showDepthMap.setUniform("zNear", DEPTH_MAP_NEAR);
    showDepthMap.setUniform("zFar", DEPTH_MAP_FAR);
    quad.draw(showDepthMap);
    */

    // render imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  // exit
  depthMap.destroy();
  shader.destroy();
  scene.destroy();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}