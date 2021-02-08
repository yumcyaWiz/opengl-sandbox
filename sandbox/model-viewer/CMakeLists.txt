add_executable(model-viewer src/model-viewer.cpp)
target_compile_features(model-viewer PRIVATE cxx_std_17)
set_target_properties(model-viewer PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(model-viewer PRIVATE src)
target_link_libraries(model-viewer PRIVATE ogls)
target_link_libraries(model-viewer PRIVATE glfw)
target_link_libraries(model-viewer PRIVATE imgui)
target_link_libraries(model-viewer PRIVATE imgui_glfw_opengl3)

# set cmake source dir macro
target_compile_definitions(model-viewer PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")