#include <filesystem>
#include <iostream>

#include "glad/glad.h"
//
#include "GLFW/glfw3.h"
//
#include "ogls.hpp"

using namespace ogls;

// globals
int width = 512;
int height = 512;

void handleInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int _width,
                               int _height)
{
  width = _width;
  height = _height;
  glViewport(0, 0, width, height);
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
  GLFWwindow *window =
      glfwCreateWindow(width, height, "triangle", nullptr, nullptr);
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

  // setup VAO
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // setup VBO
  Buffer buffer;
  float vertices[] = {
      -0.5f, -0.5f, 0.0f,  // position1
      1.0f,  0.0f,  0.0f,  // color1
      0.5f,  -0.5f, 0.0f,  // position2
      0.0f,  1.0f,  0.0f,  // color2
      0.0f,  0.5f,  0.0f,  // position3
      0.0f,  0.0f,  1.0f   // color3
  };
  buffer.setData(vertices, 18, GL_STATIC_DRAW);

  // setup VAO
  VertexArrayObject vao;
  vao.bindVertexBuffer(buffer, 0, 0, 6 * sizeof(float));
  vao.activateVertexAttribution(0, 0, 3, GL_FLOAT, 0);
  vao.activateVertexAttribution(0, 1, 3, GL_FLOAT, 3 * sizeof(float));

  // setup shader
  const Shader vertex_shader = Shader::createVertexShader(
      std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) / "shaders/shader.vert");
  const Shader fragment_shader = Shader::createFragmentShader(
      std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) / "shaders/shader.frag");

  const Pipeline pipeline;
  pipeline.attachVertexShader(vertex_shader);
  pipeline.attachFragmentShader(fragment_shader);

  // app loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    handleInput(window);

    // render
    glClear(GL_COLOR_BUFFER_BIT);
    pipeline.activate();
    vao.activate();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vao.deactivate();
    pipeline.deactivate();

    glfwSwapBuffers(window);
  }

  // exit
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}