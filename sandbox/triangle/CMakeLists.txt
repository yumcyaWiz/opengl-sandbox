add_executable(triangle src/triangle.cpp)
target_include_directories(triangle PRIVATE src)
target_link_libraries(triangle PRIVATE
    sandbox
)

# set cmake source dir macro
target_compile_definitions(triangle PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")