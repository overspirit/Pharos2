cmake_minimum_required(VERSION 3.5.0)

project(zlib)

include_directories(Source)

set(SRC_FILES
        Source/adler32.c
        Source/compress.c
        Source/crc32.c
        Source/deflate.c
        Source/infback.c
        Source/inffast.c
        Source/inflate.c
        Source/inftrees.c
        Source/trees.c
        Source/uncompr.c
        Source/zutil.c
)

set(ZIP_SRC_FILES
        Source/ioapi.c
        Source/mztools.c
        Source/unzip.c
        Source/zip.c

)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${ZIP_SRC_FILES})


