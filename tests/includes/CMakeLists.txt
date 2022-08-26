SET(
    APP_EXE includesTest
)

ADD_EXECUTABLE(
    ${APP_EXE} 
    main.cpp
)

include_directories(
  ${PROJECT_NAME}
  "../../include/"
)

target_include_directories(
    ${APP_EXE} 
    PRIVATE 
    ${SOURCE_SUBDIR}/include
)

TARGET_LINK_LIBRARIES(
    ${APP_EXE} 
    CyanShaderLib
)

add_test(
    NAME ${APP_EXE}
    COMMAND ${APP_EXE}
)