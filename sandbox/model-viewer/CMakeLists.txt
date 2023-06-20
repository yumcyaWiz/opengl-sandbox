add_executable(model-viewer src/model-viewer.cpp)
target_include_directories(model-viewer PRIVATE src)
target_link_libraries(model-viewer PRIVATE
    sandbox
)

# set cmake source dir macro
target_compile_definitions(model-viewer PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")