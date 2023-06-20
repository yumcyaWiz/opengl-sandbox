#include <filesystem>

#include "sandbox-base.hpp"

namespace sandbox
{

class NormalMapping : public SandboxBase
{
 public:
  NormalMapping(uint32_t width, uint32_t height) : SandboxBase(width, height) {}

  void beforeRender() override
  {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    scene.setPointLight({glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f});

    pipeline.loadVertexShader(std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
                              "shaders/shader.vert");
    pipeline.loadFragmentShader(
        std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
        "shaders/shader.frag");
  }

  void runImGui() override
  {
    ImGui::Begin("UI");
    {
      static char modelPath[100] = {"assets/normalmap_test/normalmap_test.obj"};
      ImGui::InputText("Model", modelPath, 100);
      if (ImGui::Button("Load Model")) {
        scene.setModel({std::string(CMAKE_SOURCE_DIR) + "/" + modelPath});
      }

      ImGui::Separator();

      ImGui::InputFloat("FOV", &camera.fov);
      ImGui::InputFloat("Movement Speed", &camera.movement_speed);
      ImGui::InputFloat("Look Around Speed", &camera.look_around_speed);

      if (ImGui::Button("Reset Camera")) { camera.reset(); }

      ImGui::Separator();

      ImGui::Checkbox("Normal Mapping", &use_normal_map);
      ImGui::Checkbox("Show Normal", &show_normal);
    }
    ImGui::End();
  }

  void handleInput() override
  {
    // close application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      camera.move(ogls::CameraMovement::FORWARD, io->DeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      camera.move(ogls::CameraMovement::LEFT, io->DeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      camera.move(ogls::CameraMovement::BACKWARD, io->DeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      camera.move(ogls::CameraMovement::RIGHT, io->DeltaTime);
    }

    // camera look around
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
      camera.lookAround(io->MouseDelta.x, io->MouseDelta.y);
    }
  }

  void render() override
  {
    // set uniform variables
    pipeline.setUniform("view", camera.computeViewMatrix());
    pipeline.setUniform("projection",
                        camera.computeProjectionMatrix(width, height));
    pipeline.setUniform("camPos", camera.cam_pos);
    pipeline.setUniform("useNormalMap", use_normal_map);
    pipeline.setUniform("showNormal", show_normal);

    // render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.draw(pipeline);
  }

 private:
  ogls::Pipeline pipeline;

  bool use_normal_map = false;
  bool show_normal = false;
};

}  // namespace sandbox

int main()
{
  sandbox::NormalMapping app(1280, 720);

  app.run();

  return 0;
}