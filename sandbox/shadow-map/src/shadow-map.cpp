#include <cstdlib>
#include <filesystem>

#include "depth-map.h"
#include "sandbox-base.hpp"

namespace sandbox
{

class ShadowMap : public SandboxBase
{
 public:
  ShadowMap(uint32_t width, uint32_t height)
      : SandboxBase(width, height), depth_map(1024, 1024)
  {
  }

  void beforeRender() override
  {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    scene.setDirectionalLight(
        {glm::vec3(1.0f), glm::normalize(glm::vec3(0.5f, 1.0f, 0.5f))});

    show_depthmap_pipeline.loadVertexShader(
        std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
        "shaders/show-depthmap.vert");
    show_depthmap_pipeline.loadFragmentShader(
        std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
        "shaders/show-depthmap.frag");

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

    if (ImGui::InputInt("Depth Map Resolution", &depth_map_res)) {
      depth_map.setResolution(depth_map_res, depth_map_res);
    }
    ImGui::InputFloat("Depth Bias", &depth_bias);
    ImGui::InputFloat("Depth Map Size", &depth_map_size);
    ImGui::InputFloat("Directional Light Distance", &light_distance);
    ImGui::InputFloat("Depth Map zNear", &depth_map_near);
    ImGui::InputFloat("Depth Map zFar", &depth_map_far);

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
    // update light direction
    t += 0.1f * io->DeltaTime;
    const glm::vec3 light_direction =
        glm::normalize(glm::vec3(0.5f * glm::cos(t), 1.0f, 0.5f * std::sin(t)));
    scene.setDirectionalLight(
        ogls::DirectionalLight(glm::vec3(1.0f), light_direction));

    // set view, projection matrix for making depth map
    const glm::mat4 lightView =
        glm::lookAt(light_distance * light_direction, glm::vec3(0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 lightProjection =
        glm::ortho(-depth_map_size, depth_map_size, -depth_map_size,
                   depth_map_size, depth_map_near, depth_map_far);
    const glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    depth_map.setLightSpaceMatrix(lightSpaceMatrix);

    // make depth map
    depth_map.draw(scene);

    // render scene with shadow mapping
    // set uniforms
    pipeline.setUniform("viewProjection",
                        camera.computeViewProjectionMatrix(width, height));
    pipeline.setUniform("lightSpaceMatrix", lightSpaceMatrix);
    pipeline.setUniform("camPos", camera.cam_pos);
    // TODO: set texture unit number appropriately
    depth_map.getTextureRef().bindToTextureUnit(10);
    pipeline.setUniform("depthMap", 10);
    pipeline.setUniform("depthBias", depth_bias);

    // render
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.draw(pipeline);

    // show depth map
    glViewport(width - 256, height - 256, 256, 256);
    glClear(GL_DEPTH_BUFFER_BIT);
    depth_map.getTextureRef().bindToTextureUnit(10);
    show_depthmap_pipeline.setUniform("depthMap", 10);
    show_depthmap_pipeline.setUniform("zNear", depth_map_near);
    show_depthmap_pipeline.setUniform("zFar", depth_map_far);
    quad.draw(show_depthmap_pipeline);
  }

 private:
  ogls::Quad quad;
  ogls::Pipeline show_depthmap_pipeline;
  ogls::Pipeline pipeline;
  DepthMap depth_map;

  float t = 0.0f;
  int depth_map_res = 1024;
  float depth_bias = 0.005f;
  float depth_map_near = 100.0f;
  float depth_map_far = 10000.0f;
  float depth_map_size = 2000.0f;
  float light_distance = 2000.0f;
};

}  // namespace sandbox

int main()
{
  sandbox::ShadowMap app(1280, 720);

  app.run();

  return 0;
}