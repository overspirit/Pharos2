cmake_minimum_required(VERSION 3.5.0)

project(jpeg)

include_directories(Source)

set(SRC_FILES
        Source/cdjpeg.c
        Source/jaricom.c
        Source/jcapimin.c
        Source/jcapistd.c
        Source/jcarith.c
        Source/jccoefct.c
        Source/jccolor.c
        Source/jcdctmgr.c
        Source/jchuff.c
        Source/jcinit.c
        Source/jcmainct.c
        Source/jcmarker.c
        Source/jcmaster.c
        Source/jcomapi.c
        Source/jcparam.c
        Source/jcprepct.c
        Source/jcsample.c
        Source/jctrans.c
        Source/jdapimin.c
        Source/jdapistd.c
        Source/jdarith.c
        Source/jdatadst.c
        Source/jdatasrc.c
        Source/jdcoefct.c
        Source/jdcolor.c
        Source/jddctmgr.c
        Source/jdhuff.c
        Source/jdinput.c
        Source/jdmainct.c
        Source/jdmarker.c
        Source/jdmaster.c
        Source/jdmerge.c
        Source/jdpostct.c
        Source/jdsample.c
        Source/jdtrans.c
        Source/jerror.c
        Source/jfdctflt.c
        Source/jfdctfst.c
        Source/jfdctint.c
        Source/jidctflt.c
        Source/jidctfst.c
        Source/jidctint.c
        Source/jmemmgr.c
        Source/jmemnobs.c
        Source/jquant1.c
        Source/jquant2.c
        Source/jutils.c
        Source/rdbmp.c
        Source/rdcolmap.c
        Source/rdgif.c
        Source/rdppm.c
        Source/rdrle.c
        Source/rdswitch.c
        Source/rdtarga.c
        Source/transupp.c
        Source/wrbmp.c
        Source/wrgif.c
        Source/wrppm.c
        Source/wrrle.c
        Source/wrtarga.c
)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})


