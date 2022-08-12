# CMakeList.txt : CMake project for CyanArgParser, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project(
    "CyanShaderLib" 
    VERSION 1.0.0 
    DESCRIPTION "A simple C++17 GLSL shader library"
)

include_directories(
  ${PROJECT_NAME}
  "include/"
)

add_library(${PROJECT_NAME} STATIC
    "src/ShaderObject.cpp"
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES 

    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/ShaderObject.hpp
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