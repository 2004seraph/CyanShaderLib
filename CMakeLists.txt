# CMakeList.txt : CMake project for CyanArgParser, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

set(
    CMAKE_CXX_STANDARD 17
)

project(
    "CyanShaderLib" 
    VERSION 1.0.0 
    DESCRIPTION "A simple C++17 GLSL shader library"
)

find_package(glew CONFIG REQUIRED)

include_directories(
  ${PROJECT_NAME}
  ${GLEW_INCLUDE_DIRS}
  "include/"
)

add_library(${PROJECT_NAME} STATIC
    "src/ShaderLoader.cpp"
    "src/ShaderSource.cpp" 
    "src/ShaderProgram.cpp")

 target_link_libraries(
     ${PROJECT_NAME} 
     GLEW::GLEW
 )

set_target_properties(
    ${PROJECT_NAME} PROPERTIES 

    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER 
    include/ShaderLoader.hpp
    include/ShaderSource.hpp
    include/ShaderProgram.hpp
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE include/
)

install(
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# TESTS

add_subdirectory(tests)