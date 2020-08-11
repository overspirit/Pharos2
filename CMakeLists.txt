cmake_minimum_required(VERSION 3.5.0)

project(Pharos2)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g2 -ggdb -std=c++11")# -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -std=c++11")# -Wall")

include(External/External.cmake)

#第三方库
foreach(external ${EXTERNAL_LIST})
    add_subdirectory(${external})
endforeach()


#引擎
set(ENGINE_DIR ${PROJECT_SOURCE_DIR}/Source)
include_directories(${ENGINE_DIR})
add_subdirectory(${ENGINE_DIR})


# set(SRC_FILES
#     MyApp.cpp
#     )

# add_executable(${PROJECT_NAME} ${SRC_FILES})

# #link
# target_link_libraries(${PROJECT_NAME} Engine)


#例子
set(SAMPLE_DIR ${PROJECT_SOURCE_DIR}/Sample)
add_subdirectory(${SAMPLE_DIR}/SceneDemo)
add_subdirectory(${SAMPLE_DIR}/ModelViewer)