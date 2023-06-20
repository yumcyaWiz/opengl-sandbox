add_executable(shadow-map src/shadow-map.cpp)
target_include_directories(shadow-map PRIVATE src)
target_link_libraries(shadow-map PRIVATE
    sandbox
)

# set cmake source dir macro
target_compile_definitions(shadow-map PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}" CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")