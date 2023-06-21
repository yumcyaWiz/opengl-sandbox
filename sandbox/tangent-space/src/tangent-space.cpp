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

    void beforeRender() override
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        pipeline.loadVertexShader(
            std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
            "shaders/shader.vert");
        pipeline.loadFragmentShader(
            std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
            "shaders/shader.frag");

        tangent_space_pipeline.loadVertexShader(
            std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
            "shaders/tangent-space.vert");
        tangent_space_pipeline.loadGeometryShader(
            std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
            "shaders/tangent-space.geom");
        tangent_space_pipeline.loadFragmentShader(
            std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
            "shaders/tangent-space.frag");
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

            ImGui::InputFloat("Line Length", &line_length);
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
        const glm::mat4 view = camera.computeViewMatrix();
        const glm::mat4 projection =
            camera.computeProjectionMatrix(width, height);
        pipeline.setUniform("view", view);
        pipeline.setUniform("projection", projection);
        tangent_space_pipeline.setUniform("view", view);
        tangent_space_pipeline.setUniform("projection", projection);
        tangent_space_pipeline.setUniform("lineLength", line_length);

        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.draw(pipeline);

        // show tangent space
        scene.draw(tangent_space_pipeline);
    }

   private:
    ogls::Pipeline pipeline;
    ogls::Pipeline tangent_space_pipeline;

    float line_length = 1.0f;
};

}  // namespace sandbox

int main()
{
    sandbox::TangentSpace app(1280, 720);

    app.run();

    return 0;
}