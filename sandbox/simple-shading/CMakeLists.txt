add_executable(simple-shading src/simple-shading.cpp)
target_include_directories(simple-shading PRIVATE src)
target_link_libraries(simple-shading PRIVATE
    sandbox
)

# set cmake source dir macro
target_compile_definitions(simple-shading PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")