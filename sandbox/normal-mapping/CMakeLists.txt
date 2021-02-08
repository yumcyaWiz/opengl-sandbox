add_executable(normal-mapping src/normal-mapping.cpp)
target_compile_features(normal-mapping PRIVATE cxx_std_17)
set_target_properties(normal-mapping PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(normal-mapping PRIVATE src)
target_link_libraries(normal-mapping PRIVATE ogls)
target_link_libraries(normal-mapping PRIVATE glfw)
target_link_libraries(normal-mapping PRIVATE imgui)
target_link_libraries(normal-mapping PRIVATE imgui_glfw_opengl3)

# set cmake source dir macro
target_compile_definitions(normal-mapping PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")