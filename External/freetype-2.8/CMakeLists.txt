cmake_minimum_required(VERSION 3.5.0)

project(freetype)

add_definitions(-DFT2_BUILD_LIBRARY)

include_directories(Include)

set(SRC_FILES
        Source/autofit/autofit.c
        Source/base/ftbase.c
        Source/base/ftbbox.c
        Source/base/ftbitmap.c
        Source/base/ftdebug.c
        Source/base/ftfntfmt.c
        Source/base/ftfstype.c
        Source/base/ftgasp.c
        Source/base/ftglyph.c
        Source/base/ftgxval.c
        Source/base/ftinit.c
        Source/base/ftlcdfil.c
        Source/base/ftmm.c
        Source/base/ftotval.c
        Source/base/ftpatent.c
        Source/base/ftpfr.c
        Source/base/ftstroke.c
        Source/base/ftsynth.c
        Source/base/ftsystem.c
        Source/base/fttype1.c
        Source/base/ftwinfnt.c
        Source/bdf/bdf.c
        Source/cache/ftcache.c
        Source/cff/cff.c
        Source/cid/type1cid.c
        Source/gzip/ftgzip.c
        Source/lzw/ftlzw.c
        Source/pcf/pcf.c
        Source/pfr/pfr.c
        Source/psaux/psaux.c
        Source/pshinter/pshinter.c
        Source/psnames/psmodule.c
        Source/raster/raster.c
        Source/sfnt/sfnt.c
        Source/smooth/smooth.c
        Source/truetype/truetype.c
        Source/type1/type1.c
        Source/type42/type42.c
        Source/winfonts/winfnt.c
)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})


