# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.
cmake_minimum_required(VERSION 3.4.1)

# set the project name
project(libmsfa VERSION 0.1)

# Specifies a library name, specifies whether the library is STATIC or
# SHARED, and provides relative paths to the source code. You can
# define multiple libraries by adding multiple add_library() commands,
# and CMake builds them for you. When you build your app, Gradle
# automatically packages shared libraries with your APK.
add_library(
    # Specifies the name of the library.
    msfa

    # Sets the library as a shared library.
    SHARED

    # Provides a relative path to your source file(s).
    src/dx7note.cc
    src/env.cc
    src/exp2.cc
    src/fir.cc
    src/fm_core.cc
    src/fm_op_kernel.cc
    src/freqlut.cc
    src/lfo.cc
    src/log2.cc
    src/patch.cc
    src/pitchenv.cc
    src/resofilter.cc
    src/ringbuffer.cc
    src/sawtooth.cc
    src/sin.cc
    src/synth_unit.cc   
)

add_library(
    # only used by the test runner executable
    testhelper

    src/wavout.cc
)

target_compile_options(msfa PRIVATE -Wno-c++11-narrowing)

include_directories("include")

target_link_libraries(
    # Specifies the target library.
    msfa
)

# add the executable
add_executable(tester src/tester.cc)

# now link the test executable with the share lib
target_link_libraries(tester testhelper msfa)

# add the executable
add_executable(sample src/main.cc)

# now link the sample executable with the share lib
target_link_libraries(sample msfa)