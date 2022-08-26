SET(
    APP_EXE basicApp
)

ADD_EXECUTABLE(
    ${APP_EXE}
    main.cpp
)

#VCPKG LIBS
find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)

include_directories(
  ${PROJECT_NAME}
  "../../include/"
  ${GLEW_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
)

target_include_directories(
    ${APP_EXE} 
    PRIVATE 
    ${SOURCE_SUBDIR}/include
)

TARGET_LINK_LIBRARIES(
    ${APP_EXE} 
    opengl32 
    winmm
    gdi32
    CyanShaderLib
#    GLEW::GLEW
    glfw
)

add_test(
    NAME ${APP_EXE}
    COMMAND ${APP_EXE}
)