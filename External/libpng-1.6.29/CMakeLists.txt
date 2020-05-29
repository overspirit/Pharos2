cmake_minimum_required(VERSION 3.5.0)

project(png)

include_directories(Source)
include_directories(../zlib-1.2.11/Source)

set(SRC_FILES
        Source/png.c
        Source/pngerror.c
        Source/pngget.c
        Source/pngmem.c
        Source/pngpread.c
        Source/pngread.c
        Source/pngrio.c
        Source/pngrtran.c
        Source/pngrutil.c
        Source/pngset.c
        Source/pngtrans.c
        Source/pngwio.c
        Source/pngwrite.c
        Source/pngwtran.c
        Source/pngwutil.c
)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} zlib)
