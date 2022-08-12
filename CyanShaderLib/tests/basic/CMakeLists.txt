SET(
    APP_EXE basicApp
)

include_directories(
  ${PROJECT_NAME}
  "../../include/"
)

ADD_EXECUTABLE(
    ${APP_EXE}
    main.cpp
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
    NAME basicApp
    COMMAND basicApp
)