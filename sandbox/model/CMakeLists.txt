add_executable(model src/model.cpp)
target_compile_features(model PRIVATE cxx_std_17)
set_target_properties(model PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(model PRIVATE src)
target_link_libraries(model PRIVATE ogls)
target_link_libraries(model PRIVATE glfw)
target_link_libraries(model PRIVATE imgui)
target_link_libraries(model PRIVATE imgui_glfw_opengl3)

# set cmake source dir macro
target_compile_definitions(model PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# copy shaders under build directory
add_custom_command(TARGET model POST_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:model>/shaders
  COMMENT "copying shaders" VERBATIM
)