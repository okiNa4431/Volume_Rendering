set(BUILD_TARGET "volume_rendering")

#実行ファイル生成
add_executable(${BUILD_TARGET})
target_sources(${BUILD_TARGET} PUBLIC "main.cpp" "application.h" "application.cpp" "io.h" "renderer.h" "renderer.cpp")

#include, lib
target_include_directories(${BUILD_TARGET} PUBLIC /Libraries/glfwlib/include/GLFW)
target_link_libraries(${BUILD_TARGET}
  OpenGL::GL
  glfw
  ${CMAKE_DL_LIBS}
)