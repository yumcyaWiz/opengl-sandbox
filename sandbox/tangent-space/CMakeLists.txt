add_executable(tangent-space src/tangent-space.cpp)
target_compile_features(tangent-space PRIVATE cxx_std_17)
set_target_properties(tangent-space PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(tangent-space PRIVATE src)
target_link_libraries(tangent-space PRIVATE ogls)
target_link_libraries(tangent-space PRIVATE glfw)
target_link_libraries(tangent-space PRIVATE imgui)
target_link_libraries(tangent-space PRIVATE imgui_glfw_opengl3)

# set cmake source dir macro
target_compile_definitions(tangent-space PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")