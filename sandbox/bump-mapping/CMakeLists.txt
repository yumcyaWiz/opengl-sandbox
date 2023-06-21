add_executable(bump-mapping src/bump-mapping.cpp)
target_include_directories(bump-mapping PRIVATE src)
target_link_libraries(bump-mapping PRIVATE
    sandbox
)

# set cmake source dir macro
target_compile_definitions(bump-mapping PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")