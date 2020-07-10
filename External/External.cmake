#message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
#message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")

#第三方库
set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/External)

#message(STATUS "EXTERNAL_DIR: ${EXTERNAL_DIR}")


set(EXTERNAL_LIST)
#list(APPEND EXTERNAL_LIST ${EXTERNAL_DIR}/glslang)
list(APPEND EXTERNAL_LIST ${EXTERNAL_DIR}/zlib-1.2.11)
list(APPEND EXTERNAL_LIST ${EXTERNAL_DIR}/libpng-1.6.29)
list(APPEND EXTERNAL_LIST ${EXTERNAL_DIR}/libjpeg-9b)
list(APPEND EXTERNAL_LIST ${EXTERNAL_DIR}/freetype-2.8)
list(APPEND EXTERNAL_LIST ${EXTERNAL_DIR}/FreeImage-3.17.0)


set(EXTERNAL_LIBS)
#list(APPEND EXTERNAL_LIBS glslang)
#list(APPEND EXTERNAL_LIBS glslang-default-resource-limits)
#list(APPEND EXTERNAL_LIBS SPIRV)
list(APPEND EXTERNAL_LIBS zlib)
list(APPEND EXTERNAL_LIBS png)
list(APPEND EXTERNAL_LIBS jpeg)
list(APPEND EXTERNAL_LIBS freetype)
list(APPEND EXTERNAL_LIBS FreeImage)


#include_directories(${EXTERNAL_DIR}/glslang)
include_directories(${EXTERNAL_DIR}/zlib-1.2.11/Source)
include_directories(${EXTERNAL_DIR}/libpng-1.6.29/Source)
include_directories(${EXTERNAL_DIR}/libjpeg-9b/Source)
include_directories(${EXTERNAL_DIR}/freetype-2.8/Include)
include_directories(${EXTERNAL_DIR}/FreeImage-3.17.0/Include)
include_directories(${EXTERNAL_DIR}/rapidXml)
include_directories(${EXTERNAL_DIR}/rapidjson)


if (NOT ANDROID)
    list(APPEND EXTERNAL_LIST ${EXTERNAL_DIR}/glfw)
    list(APPEND EXTERNAL_LIBS glfw)
    include_directories(${EXTERNAL_DIR}/glfw/include)
endif()

