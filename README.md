# opengl-sandbox

This is an experimental environment for implementing various rendering methods in OpenGL.

## Requirements

* C++20
* Clang(Currently, GCC compiler doesn't work)
* CMake 3.12 or Higher
* OpenGL 4.6

## Build

```bash
git submodule update --init --recursive
mkdir build
cd build
CC=clang CXX=clang++ cmake ..
make
```

## Download assets

```bash
./scripts/download_assets.sh
```

## How to add your example

You can add your example by implementing `SandboxBase` interface like this.

```cpp
#include <filesystem>

#include "sandbox-base.hpp"

namespace sandbox
{
class Triangle : public SandboxBase
{
   public:
    Triangle(uint32_t width, uint32_t height) : SandboxBase(width, height) {}

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

   private:
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
```

See `sandbox` directory for details.

## Gallery

### triangle

Hello world in OpenGL.

![](./img/triangle.jpg)

### model-viewer

Visualize 3d models with various layers such as Position, Normal, TexCoords, Diffuse, ...

![](./img/model-viewer.jpg)

### tangent-space

Visualize basis of tangent space at each vertex using geometry shader.

![](./img/tangent-space.jpg)

### simple-shading

Blinn-Phong shading example with moving point light.

![](./img/simple-shading.jpg)

### normal-mapping

Normal mapping example with Blinn-Phong shading.

![](./img/normal-mapping-compare.jpg)

### bump-mapping

Bump mapping example using height map with Blinn-Phong shading.

![](./img/bump-mapping-compare.jpg)

### shadow-mapping

Shadow mapping example with moving directional light.

![](./img/shadow-mapping3.jpg)

### omnidirectional-shadow-mapping

Omnidirectional shadow mapping example with moving point light.

![](./img/omnidirectional-shadow-mapping.jpg)

## Externals

* [glad](https://github.com/Dav1dde/glad)
* [glm](https://github.com/g-truc/glm)
* [GLFW](https://github.com/glfw/glfw)
* [assimp](https://github.com/assimp/assimp)
* [imgui](https://github.com/ocornut/imgui)
* [stb](https://github.com/nothings/stb)
* [GLSL Shader Includes](https://github.com/tntmeijs/GLSL-Shader-Includes)

## References

* [Learn OpenGL](https://learnopengl.com/)
* [Project ASURA](http://www.project-asura.com/)
