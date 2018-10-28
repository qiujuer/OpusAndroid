# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

aux_source_directory(src/main/cpp/opus/include LIB_OPUS_INCLUDE)
aux_source_directory(src/main/cpp/opus/silk LIB_SILK)
aux_source_directory(src/main/cpp/opus/silk/fixed LIB_SILK_FIXED)
aux_source_directory(src/main/cpp/opus/silk/float LIB_SILK_FLOAT)
aux_source_directory(src/main/cpp/opus/celt LIB_CELT)
aux_source_directory(src/main/cpp/opus/src LIB_SRC)
aux_source_directory(src/main/cpp/opus/ LIB_OPUS_ROOT)
include_directories(src/main/cpp/opus/include
                    src/main/cpp/opus/silk
                    src/main/cpp/opus/silk/fixed
                    src/main/cpp/opus/silk/float
                    src/main/cpp/opus/celt
                    src/main/cpp/opus/src
                    src/main/cpp/opus/)

add_library( # Sets the name of the library.
        opus-lib
        # Sets the library as a shared library.
        SHARED

        src/main/cpp/native-lib.cpp
        ${LIB_OPUS_INCLUDE}
        ${LIB_SILK}
        ${LIB_SILK_FIXED}
        ${LIB_SILK_FLOAT}
        ${LIB_CELT}
        ${LIB_SRC}
        ${LIB_OPUS_ROOT})

add_definitions(-DOPUS_BUILD -DSTDC_HEADERS -DVAR_ARRAYS)
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
        opus-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} )