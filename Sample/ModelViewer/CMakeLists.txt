cmake_minimum_required(VERSION 3.5.0)

project(ModelViewer)

set(SRC_FILES
    MyApp.cpp
    )

if (ANDROID)

    add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

else()

    add_executable(${PROJECT_NAME} ${SRC_FILES})

endif()


#link
target_link_libraries(${PROJECT_NAME} Engine)


if (ANDROID)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-u ANativeActivity_onCreate")
endif()