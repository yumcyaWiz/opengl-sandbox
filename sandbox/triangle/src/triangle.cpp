#include <iostream>

#include "glad/glad.h"
//
#include "GLFW/glfw3.h"
//
#include "ogls/shader.hpp"

using namespace ogls;

// globals
int width = 512;
int height = 512;

void handleInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int _width,
                             int _height) {
  width = _width;
  height = _height;
  glViewport(0, 0, width, height);
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
  GLFWwindow* window =
      glfwCreateWindow(width, height, "triangle", nullptr, nullptr);
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

  // setup VAO
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // setup VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  float vertices[] = {
      -0.5f, -0.5f, 0.0f,  // position1
      1.0f,  0.0f,  0.0f,  // color1
      0.5f,  -0.5f, 0.0f,  // position2
      0.0f,  1.0f,  0.0f,  // color2
      0.0f,  0.5f,  0.0f,  // position3
      0.0f,  0.0f,  1.0f   // color3
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void*>(0));

  // color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void*>(3 * sizeof(float)));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // setup shader
  Shader shader{"shaders/shader.vert", "shaders/shader.frag"};

  // app loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    handleInput(window);

    // render
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    shader.activate();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    shader.deactivate();
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  // exit
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}