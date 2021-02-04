cmake_minimum_required(VERSION 3.12)
project(realtime-graphics-sandbox LANGUAGES C CXX)

# externals
add_subdirectory(externals)

# OpenGL
find_package(OpenGL REQUIRED)

# assimp
find_package(ASSIMP REQUIRED)

# common library
add_library(rtgs src/shader.cpp)
target_compile_features(rtgs PUBLIC cxx_std_17)
set_target_properties(rtgs PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(rtgs PUBLIC include)
target_link_libraries(rtgs PUBLIC OpenGL::GL)
target_link_libraries(rtgs PUBLIC glad)
target_link_libraries(rtgs PUBLIC glm)
target_link_libraries(rtgs PUBLIC assimp)
target_link_libraries(rtgs PUBLIC stb)