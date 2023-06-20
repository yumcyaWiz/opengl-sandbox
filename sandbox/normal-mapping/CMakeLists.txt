add_executable(normal-mapping src/normal-mapping.cpp)
target_include_directories(normal-mapping PRIVATE src)
target_link_libraries(normal-mapping PRIVATE
    sandbox
)

# set cmake source dir macro
target_compile_definitions(normal-mapping PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")