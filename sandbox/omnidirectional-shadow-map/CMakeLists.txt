add_executable(omnidirectional-shadow-map src/omnidirectional-shadow-map.cpp)
target_compile_features(omnidirectional-shadow-map PRIVATE cxx_std_17)
set_target_properties(omnidirectional-shadow-map PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(omnidirectional-shadow-map PRIVATE src)
target_link_libraries(omnidirectional-shadow-map PRIVATE ogls)
target_link_libraries(omnidirectional-shadow-map PRIVATE glfw)
target_link_libraries(omnidirectional-shadow-map PRIVATE imgui)
target_link_libraries(omnidirectional-shadow-map PRIVATE imgui_glfw_opengl3)

# set cmake source dir macro
target_compile_definitions(omnidirectional-shadow-map PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")