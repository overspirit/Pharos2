cmake_minimum_required(VERSION 3.5.0)

project(SceneDemo)

set(SRC_FILES
    MyApp.cpp
    )

add_executable(${PROJECT_NAME} ${SRC_FILES})

#link
target_link_libraries(${PROJECT_NAME} Engine)