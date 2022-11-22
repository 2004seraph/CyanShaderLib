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

find_package(
    GLEW REQUIRED
)

include_directories(
  ${PROJECT_NAME}
  "include/"
)

add_library(${PROJECT_NAME} STATIC
    src/ShaderLoader.cpp
    src/ShaderSource.cpp 
    src/ShaderProgram.cpp)

 target_link_libraries(
     ${PROJECT_NAME} 
 PUBLIC
     GLEW::GLEW
 )

 #MSVC debugging tools like edit and continue
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

set_target_properties(
    ${PROJECT_NAME} PROPERTIES 

    VERSION ${PROJECT_VERSION}
    SOVERSION 1.0.0
    PUBLIC_HEADER CyanShaderLib/ShaderLoader.hpp CyanShaderLib/ShaderSource.hpp CyanShaderLib/ShaderProgram.hpp
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/
)

#install(
#    TARGETS ${PROJECT_NAME}
#    PUBLIC_HEADER CyanShaderLib/ShaderLoader.hpp CyanShaderLib/ShaderSource.hpp CyanShaderLib/ShaderProgram.hpp
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}
#)

# TESTS

add_subdirectory(tests)