cmake_minimum_required(VERSION 3.12)
project(realtime-graphics-sandbox LANGUAGES C CXX)

# externals
add_subdirectory(externals)

# OpenGL
find_package(OpenGL REQUIRED)

# assimp
find_package(ASSIMP REQUIRED)

# ogls
add_library(ogls src/shader.cpp src/camera.cpp src/texture.cpp)
target_compile_features(ogls PUBLIC cxx_std_17)
set_target_properties(ogls PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(ogls PUBLIC include)

# compile options
target_compile_options(ogls PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

# link externals
target_link_libraries(ogls PUBLIC OpenGL::GL)
target_link_libraries(ogls PUBLIC glad)
target_link_libraries(ogls PUBLIC glm)
target_link_libraries(ogls PUBLIC assimp)
target_link_libraries(ogls PUBLIC stb)

# sandbox
add_subdirectory(sandbox)