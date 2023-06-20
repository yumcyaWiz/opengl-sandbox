#include <filesystem>

#include "sandbox-base.hpp"

namespace sandbox
{

class ModelViewer : public SandboxBase
{
 public:
  ModelViewer(uint32_t width, uint32_t height) : SandboxBase(width, height) {}

 private:
  enum class LayerType {
    Position,
    Normal,
    TexCoords,
    Tangent,
    Dndu,
    Dndv,
    Diffuse,
    Specular,
    Ambient,
    Emissive,
    Height,
    NormalMap,
    Shininess,
    Displacement,
    Light,
  };

  void beforeRender() override
  {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    pipeline.loadVertexShader(std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
                              "shaders/shader.vert");
    pipeline.loadFragmentShader(
        std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
        "shaders/shader.frag");
  }

  void runImGui() override
  {
    ImGui::Begin("UI");

    static char modelPath[100] = {"assets/sponza/sponza.obj"};
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

    ImGui::Combo("Layer Type", reinterpret_cast<int *>(&layerType),
                 "Position\0Normal\0TexCoords\0Tangent\0dndu\0dndv\0Diffuse\0Sp"
                 "ecular\0Ambient\0Emis"
                 "sive\0Height\0NormalMap\0Shininess\0Displacement\0Light\0\0");

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
    pipeline.setUniform("layerType", static_cast<GLint>(layerType));

    // render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.draw(pipeline);
  }

  ogls::Pipeline pipeline;
  LayerType layerType = LayerType::Normal;
};

}  // namespace sandbox

int main()
{
  sandbox::ModelViewer app(1280, 720);

  app.run();

  return 0;
}