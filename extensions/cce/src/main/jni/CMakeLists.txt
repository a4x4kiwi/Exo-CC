# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.6)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__ANDROID__ -DANDROID -DCUSTOM_FLAG")
#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGL_GLEXT_PROTOTYPES=1 -DIOAPI_NO_64 -DUSE_FILE32API ")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

# Specifies a library name, specifies whether the library is STATIC or
# SHARED, and provides relative paths to the source code. You can
# define multiple libraries by adding multiple add_library() commands,
# and CMake builds them for you. When you build your app, Gradle
# automatically packages shared libraries with your APK.

add_library(
        # Specifies the name of the library.
        ccejni

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ccejni.c
        cce.c
        lib_ccx/activity.c
        lib_ccx/asf_functions.c
        lib_ccx/avc_functions.c
        lib_ccx/cc_bitstream.c
        lib_ccx/ccx_common_char_encoding.c
        lib_ccx/ccx_common_common.c
        lib_ccx/ccx_common_constants.c
        lib_ccx/ccx_common_option.c
        lib_ccx/ccx_common_timing.c
        lib_ccx/ccx_decoders_608.c
        lib_ccx/ccx_decoders_708.c
        lib_ccx/ccx_decoders_708_encoding.c
        lib_ccx/ccx_decoders_708_output.c
        lib_ccx/ccx_decoders_common.c
        lib_ccx/ccx_decoders_isdb.c
        #lib_ccx/ccx_decoders_vbi.c
        lib_ccx/ccx_decoders_xds.c
        lib_ccx/ccx_demuxer.c
        lib_ccx/ccx_demuxer_mxf.c
        lib_ccx/ccx_dtvcc.c
        lib_ccx/ccx_encoders_common.c
        lib_ccx/ccx_encoders_curl.c
        lib_ccx/ccx_encoders_g608.c
        lib_ccx/ccx_encoders_helpers.c
        lib_ccx/ccx_encoders_mcc.c
        lib_ccx/ccx_encoders_sami.c
        lib_ccx/ccx_encoders_scc.c
        lib_ccx/ccx_encoders_smptett.c
        lib_ccx/ccx_encoders_splitbysentence.c
        lib_ccx/ccx_encoders_spupng.c
        lib_ccx/ccx_encoders_srt.c
        lib_ccx/ccx_encoders_ssa.c
        lib_ccx/ccx_encoders_transcript.c
        lib_ccx/ccx_encoders_webvtt.c
        lib_ccx/ccx_decoders_xds.c
        lib_ccx/ccx_gxf.c
        lib_ccx/ccx_share.c
        lib_ccx/ccx_sub_entry_message.pb-c.c
        lib_ccx/configuration.c
        lib_ccx/dvb_subtitle_decoder.c
        lib_ccx/dvd_subtitle_decoder.c
        lib_ccx/es_functions.c
        lib_ccx/es_userdata.c
        lib_ccx/ffmpeg_intgr.c
        lib_ccx/file_functions.c
        lib_ccx/general_loop.c
        lib_ccx/hardsubx.c
        lib_ccx/hardsubx_classifier.c
        lib_ccx/hardsubx_decoder.c
        lib_ccx/hardsubx_imgops.c
        lib_ccx/hardsubx_utility.c
        lib_ccx/lib_ccx.c
        lib_ccx/matroska.c
        lib_ccx/myth.c
        lib_ccx/networking.c
        lib_ccx/ocr.c
        lib_ccx/output.c
        lib_ccx/params.c
        lib_ccx/params_dump.c
        lib_ccx/sequencing.c
        lib_ccx/stream_functions.c
        lib_ccx/telxcc.c
        lib_ccx/ts_functions.c
        lib_ccx/ts_info.c
        lib_ccx/ts_tables.c
        lib_ccx/ts_tables_epg.c
        lib_ccx/utility.c
        lib_ccx/wtv_functions.c

        #thirdparty/win_iconv/win_iconv.c
)

SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fPIE -pie")


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       ccejni

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )


# Specifies a path to native header files.
include_directories("${PROJECT_SOURCE_DIR}/")
include_directories(lib_ccx)
include_directories(thirdparty)
include_directories(thirdparty/gpacmp4)
include_directories(thirdparty/protobuf-c)
include_directories(thirdparty/zvbi)
include_directories(thirdparty/lib_hash)
include_directories(thirdparty/libpng)
include_directories(thirdparty/zlib)
include_directories(thirdparty/freetype/include)
include_directories(thirdparty/protobuf-c/)

