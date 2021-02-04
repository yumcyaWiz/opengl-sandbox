add_executable(triangle src/triangle.cpp)
target_compile_features(triangle PRIVATE cxx_std_17)
set_target_properties(triangle PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(triangle PRIVATE src)
target_link_libraries(triangle PRIVATE ogls)
target_link_libraries(triangle PRIVATE glfw)

add_custom_command(TARGET triangle POST_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:triangle>/shaders
  COMMENT "copying shaders" VERBATIM
)