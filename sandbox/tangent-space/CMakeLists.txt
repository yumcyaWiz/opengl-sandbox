add_executable(tangent-space src/tangent-space.cpp)
target_include_directories(tangent-space PRIVATE src)
target_link_libraries(tangent-space PRIVATE
    sandbox
)

# set cmake source dir macro
target_compile_definitions(tangent-space PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")