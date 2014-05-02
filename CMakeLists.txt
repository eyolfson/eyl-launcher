# Copyright 2014 Jon Eyolfson
#
# This file is distributed under the GPLv3 license

cmake_minimum_required(VERSION 2.8.12)

project(LNCHR)
set(LNCHR_VERSION_MAJOR 0)
set(LNCHR_VERSION_MINOR 0)
set(LNCHR_VERSION_PATCH 1)
set(LNCHR_VERSION_EXTRA -development)

if(EXISTS "${LNCHR_SOURCE_DIR}/.git")
    find_package(Git)
endif()
if(GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE}
                    --git-dir=${LNCHR_SOURCE_DIR}/.git
                    --work-tree=${LNCHR_SOURCE_DIR}
                    describe
                    --match "v[0-9]*" --dirty=-dirty
                    OUTPUT_VARIABLE GIT_OUTPUT
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    if("${GIT_OUTPUT}" MATCHES "^v[0-9]+\\.[0-9]+\\.[0-9]+.*$")
        string(REGEX REPLACE "^v([0-9]+).*$" "\\1"
               LNCHR_VERSION_MAJOR "${GIT_OUTPUT}")
        string(REGEX REPLACE "^v[^.]\\.([0-9]+).*$" "\\1"
               LNCHR_VERSION_MINOR "${GIT_OUTPUT}")
        string(REGEX REPLACE "^v[^.]\\.[^.]\\.([0-9]+).*$" "\\1"
               LNCHR_VERSION_PATCH "${GIT_OUTPUT}")
        string(REGEX REPLACE "^v[^.]\\.[^.]\\.[0-9]+(.*)$" "\\1"
               LNCHR_VERSION_EXTRA "${GIT_OUTPUT}")
    endif()
endif()

find_package(PkgConfig)
pkg_check_modules(PANGOCAIRO pangocairo)
pkg_check_modules(SDL2 sdl2)

add_compile_options(
    -std=c++11
    -fno-exceptions
    -fno-rtti
    -Wall
    -O2
)
set(EXECUTABLE_OUTPUT_PATH ${LNCHR_BINARY_DIR}/bin)

add_subdirectory(src)
