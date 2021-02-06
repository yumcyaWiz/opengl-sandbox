add_executable(triangle src/triangle.cpp)
target_compile_features(triangle PRIVATE cxx_std_17)
set_target_properties(triangle PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(triangle PRIVATE src)
target_link_libraries(triangle PRIVATE ogls)
target_link_libraries(triangle PRIVATE glfw)

# set cmake source dir macro
target_compile_definitions(triangle PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")