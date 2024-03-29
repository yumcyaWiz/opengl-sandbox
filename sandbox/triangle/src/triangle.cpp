#include <filesystem>

#include "sandbox-base.hpp"

namespace sandbox
{
class Triangle : public SandboxBase
{
   public:
    Triangle(uint32_t width, uint32_t height) : SandboxBase(width, height) {}

   private:
    void beforeRender() override
    {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,  // position1
            1.0f,  0.0f,  0.0f,  // color1
            0.5f,  -0.5f, 0.0f,  // position2
            0.0f,  1.0f,  0.0f,  // color2
            0.0f,  0.5f,  0.0f,  // position3
            0.0f,  0.0f,  1.0f   // color3
        };
        buffer.setData(vertices, 18, GL_STATIC_DRAW);

        vao.bindVertexBuffer(buffer, 0, 0, 6 * sizeof(float));
        vao.activateVertexAttribution(0, 0, 3, GL_FLOAT, 0);
        vao.activateVertexAttribution(0, 1, 3, GL_FLOAT, 3 * sizeof(float));

        pipeline.loadVertexShader(
            std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
            "shaders/shader.vert");
        pipeline.loadFragmentShader(
            std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) /
            "shaders/shader.frag");
    }

    void runImGui() override {}

    void handleInput() override
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void render() override
    {
        glClear(GL_COLOR_BUFFER_BIT);
        pipeline.activate();
        vao.activate();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        vao.deactivate();
        pipeline.deactivate();
    }

    ogls::Buffer buffer;
    ogls::VertexArrayObject vao;
    ogls::Pipeline pipeline;
};

}  // namespace sandbox

int main()
{
    sandbox::Triangle app(1280, 720);

    app.run();

    return 0;
}