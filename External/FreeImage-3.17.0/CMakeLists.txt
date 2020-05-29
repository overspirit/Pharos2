cmake_minimum_required(VERSION 3.5.0)

project(FreeImage)


set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wno-error=deprecated-declarations -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wno-error=deprecated-declarations -Wno-deprecated-declarations")

add_definitions(-DFREEIMAGE_LIB)

include_directories(Include)
include_directories(../libjpeg-9b/Source)
include_directories(../libpng-1.6.29/Source)
include_directories(../zlib-1.2.11/Source)

set(SRC_FILES
        Source/Background.cpp
        Source/BitmapAccess.cpp
        Source/BSplineRotate.cpp
        Source/CacheFile.cpp
        Source/Channels.cpp
        Source/ClassicRotate.cpp
        Source/ColorLookup.cpp
        Source/Colors.cpp
        Source/Conversion.cpp
        Source/Conversion16_555.cpp
        Source/Conversion16_565.cpp
        Source/Conversion24.cpp
        Source/Conversion32.cpp
        Source/Conversion4.cpp
        Source/Conversion8.cpp
        Source/ConversionFloat.cpp
        Source/ConversionRGB16.cpp
        Source/ConversionRGBA16.cpp
        Source/ConversionRGBAF.cpp
        Source/ConversionRGBF.cpp
        Source/ConversionType.cpp
        Source/ConversionUINT16.cpp
        Source/CopyPaste.cpp
        Source/Deprecated.cpp
        Source/DeprecationMgr.cpp
        Source/Display.cpp
        Source/Exif.cpp
        Source/FIRational.cpp
        Source/Flip.cpp
        Source/FreeImage.cpp
        Source/FreeImageC.c
        Source/FreeImageIO.cpp
        Source/FreeImageTag.cpp
        Source/GetType.cpp
        Source/Halftoning.cpp
        Source/IPTC.cpp
        Source/JPEGTransform.cpp
        Source/LFPQuantizer.cpp
        Source/MemoryIO.cpp
        Source/MNGHelper.cpp
        Source/MultigridPoissonSolver.cpp
        Source/MultiPage.cpp
        Source/NNQuantizer.cpp
        Source/PixelAccess.cpp
        Source/Plugin.cpp
        Source/PluginBMP.cpp
        Source/PluginDDS.cpp
        Source/PluginGIF.cpp
        Source/PluginICO.cpp
        Source/PluginJPEG.cpp
        Source/PluginPNG.cpp
        Source/PluginTARGA.cpp
        Source/PSDParser.cpp
        Source/Rescale.cpp
        Source/Resize.cpp
        Source/TagConversion.cpp
        Source/TagLib.cpp
        Source/TIFFLogLuv.cpp
        Source/tmoColorConvert.cpp
        Source/tmoDrago03.cpp
        Source/tmoFattal02.cpp
        Source/tmoReinhard05.cpp
        Source/ToneMapping.cpp
        Source/WuQuantizer.cpp
        Source/ZLibInterface.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} jpeg png zlib)

