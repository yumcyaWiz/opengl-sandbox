cmake_minimum_required(VERSION 3.12)
project(opengl-sandbox LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ version and std
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# externals
add_subdirectory(externals)

# OpenGL
find_package(OpenGL REQUIRED)

# ogls
add_library(ogls
  src/buffer.cpp
  src/camera.cpp
  src/framebuffer.cpp
  src/texture.cpp
  src/mesh.cpp
  src/model.cpp
  src/scene.cpp
  src/quad.cpp
  src/shader.cpp
  src/texture.cpp
  src/vertex-array-object.cpp
)
target_include_directories(ogls PUBLIC src/)

# compile options
target_compile_options(ogls PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -pedantic>
)

# link externals
target_link_libraries(ogls PUBLIC OpenGL::GL)
target_link_libraries(ogls PUBLIC glad)
target_link_libraries(ogls PUBLIC glm)
target_link_libraries(ogls PUBLIC assimp::assimp)
target_link_libraries(ogls PUBLIC stb)
target_link_libraries(ogls PUBLIC glsl-shader-includes)
target_link_libraries(ogls PUBLIC spdlog::spdlog)

# sandbox
add_subdirectory(sandbox)
