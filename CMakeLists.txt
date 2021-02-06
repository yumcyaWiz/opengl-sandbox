cmake_minimum_required(VERSION 3.12)
project(opengl-sandbox LANGUAGES C CXX)

# externals
add_subdirectory(externals)

# OpenGL
find_package(OpenGL REQUIRED)

# ogls
add_library(ogls 
  src/shader.cpp 
  src/camera.cpp 
  src/texture.cpp 
  src/mesh.cpp 
  src/model.cpp 
  src/scene.cpp
  src/quad.cpp
)
target_compile_features(ogls PUBLIC cxx_std_17)
set_target_properties(ogls PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(ogls PUBLIC include)

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

# sandbox
add_subdirectory(sandbox)