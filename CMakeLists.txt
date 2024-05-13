cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

# サポートされている場合は、MSVC コンパイラのホット リロードを有効にします。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Volume_Rendering")

#library to use
find_package(OpenGL REQUIRED)
set(CMAKE_PREFIX_PATH /Libraries/glfwlib)
find_package(glfw3 REQUIRED)

#subdirectory
add_subdirectory ("src")

#message
message(STATUS "----------Message Start----------")
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Cmake Version: ${CMAKE_VERSION}")
message(STATUS "Cmake Build Type: ${CMAKE_BUILD_TYPE}")
if(${OpenGL_FOUND})
	message(STATUS "OpenGL:")
	message(STATUS "OpenGL version: ")
else(OpenGL_FOUND)
	message(WARNING "OpenGL NOT Found")
endif()
if(glfw3_FOUND)
	message(STATUS "glfw3:")
else(glfw3_FOUND)
	message(WARNING "glfw3 NOT Found")
endif()
message(STATUS "----------Message End------------")