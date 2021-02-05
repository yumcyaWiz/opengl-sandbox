add_executable(simple-shading src/simple-shading.cpp)
target_compile_features(simple-shading PRIVATE cxx_std_17)
set_target_properties(simple-shading PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(simple-shading PRIVATE src)
target_link_libraries(simple-shading PRIVATE ogls)
target_link_libraries(simple-shading PRIVATE glfw)
target_link_libraries(simple-shading PRIVATE imgui)
target_link_libraries(simple-shading PRIVATE imgui_glfw_opengl3)

# set cmake source dir macro
target_compile_definitions(simple-shading PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")