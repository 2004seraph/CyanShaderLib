# CMakeList.txt : CMake project for CyanArgParser, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(
    CMAKE_CXX_STANDARD 20
)
set (HEADER
    ${HEADER}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/CyanShaderLib/ShaderLoader.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/CyanShaderLib/ShaderSource.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/CyanShaderLib/ShaderProgram.hpp
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
    PUBLIC_HEADER ${HEADER}
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>  
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>  # <prefix>/include/mylib
)

install(
    TARGETS ${PROJECT_NAME} 
    EXPORT ${PROJECT_NAME}-targets
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
    EXPORT ${PROJECT_NAME}-targets
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})

# TESTS

add_subdirectory(tests)