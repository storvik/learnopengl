cmake_minimum_required(VERSION 3.10)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project(learnopengl
  VERSION 0.0.1
  LANGUAGES C CXX
  DESCRIPTION "C++ library description")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)                          # Must be called before adding tests but after calling project(). This automatically calls enable_testing() and configures ctest targets when using Make/Ninja
include(GNUInstallDirs)                 # This will define the default values for installation directories (all platforms even if named GNU)
include(InstallRequiredSystemLibraries) # Tell CMake that the `install` target needs to install required system libraries (eg: Windows SDK)

option(ENABLE_TESTING "Should unit tests be compiled." ON)

set(GLAD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad")

find_package(spdlog REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3 REQUIRED)

add_library(glad
  STATIC
  ${GLAD_PATH}/src/glad.c)
target_include_directories(glad
  PRIVATE
  ${GLAD_PATH}/include)

# Conditional testing stuff if needed
# if(ENABLE_TESTING)
#   find_package(GTest REQUIRED)
#   include(GoogleTest)
#   enable_testing()
# endif()

set(${PROJECT_NAME}_SOURCES
  src/main.cpp)

set(${PROJECT_NAME}_INCLUDES
  ${GLAD_PATH}/include)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
  ${${PROJECT_NAME}_SOURCES})

# target includes
target_include_directories(${PROJECT_NAME}
  PRIVATE
  include/
  ${${PROJECT_NAME}_INCLUDES}
  ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  spdlog::spdlog
  glad
  ${CMAKE_DL_LIBS} # needed for glad
  glfw)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_BINDIR})

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
