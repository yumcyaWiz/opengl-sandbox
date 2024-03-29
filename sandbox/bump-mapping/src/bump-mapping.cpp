#include <filesystem>

#include "sandbox-base.hpp"

namespace sandbox
{

class TangentSpace : public SandboxBase
{
   public:
    TangentSpace(uint32_t width, uint32_t height) : SandboxBase(width, height)
    {
    }

   private:
    void beforeRender() override
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        scene.setPointLight({glm::vec3(10000), glm::vec3(0, 100, 0), 0});

        pipeline.loadVertexShader(
            std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
            "shaders/shader.vert");
        pipeline.loadFragmentShader(
            std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
            "shaders/shader.frag");
    }

    void runImGui() override
    {
        ImGui::Begin("UI");
        {
            static char modelPath[100] = {"assets/sponza/sponza.obj"};
            ImGui::InputText("Model", modelPath, 100);
            if (ImGui::Button("Load Model")) {
                scene.setModel(
                    {std::string(CMAKE_SOURCE_DIR) + "/" + modelPath});
            }

            ImGui::Separator();

            ImGui::InputFloat("FOV", &camera.fov);
            ImGui::InputFloat("Movement Speed", &camera.movement_speed);
            ImGui::InputFloat("Look Around Speed", &camera.look_around_speed);

            if (ImGui::Button("Reset Camera")) { camera.reset(); }

            ImGui::Separator();

            ImGui::Checkbox("Use Height Map", &use_height_map);
            ImGui::Combo("Height Map Method", &height_map_method,
                         "Strict\0Usual\0\0");
            ImGui::InputFloat("Height Map Scale", &height_map_scale);
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
        // set light position
        t += io->DeltaTime;
        scene.setPointLight({glm::vec3(10000.0f),
                             camera.cam_pos + 200.0f * camera.cam_forward,
                             0.0f});

        // set uniform variables
        const glm::mat4 view = camera.computeViewMatrix();
        const glm::mat4 projection =
            camera.computeProjectionMatrix(width, height);
        pipeline.setUniform("view", view);
        pipeline.setUniform("projection", projection);
        pipeline.setUniform("camPos", camera.cam_pos);
        pipeline.setUniform("useHeightMap", use_height_map);
        pipeline.setUniform("heightMapMethod", height_map_method);
        pipeline.setUniform("heightMapScale", height_map_scale);

        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.draw(pipeline);
    }

    ogls::Pipeline pipeline;

    float t = 0.0f;
    bool use_height_map = false;
    int height_map_method = 1;
    float height_map_scale = 0.01f;
};

}  // namespace sandbox

int main()
{
    sandbox::TangentSpace app(1280, 720);

    app.run();

    return 0;
}