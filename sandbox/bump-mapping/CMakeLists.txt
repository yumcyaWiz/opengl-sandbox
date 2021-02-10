add_executable(bump-mapping src/bump-mapping.cpp)
target_compile_features(bump-mapping PRIVATE cxx_std_17)
set_target_properties(bump-mapping PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(bump-mapping PRIVATE src)
target_link_libraries(bump-mapping PRIVATE ogls)
target_link_libraries(bump-mapping PRIVATE glfw)
target_link_libraries(bump-mapping PRIVATE imgui)
target_link_libraries(bump-mapping PRIVATE imgui_glfw_opengl3)

# set cmake source dir macro
target_compile_definitions(bump-mapping PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")