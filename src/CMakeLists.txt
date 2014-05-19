# Copyright 2014 Jon Eyolfson
#
# This file is distributed under the GPLv3 license

include_directories(
    ${LNCHR_SOURCE_DIR}/include
    ${PANGOCAIRO_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

configure_file(version.cpp.in version.cpp)

add_executable(lnchr
    lnchr.cpp
    xdg.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
)

add_executable(eyl-xdg
    xdg-main.cpp
    xdg.cpp
)

target_link_libraries(lnchr
    ${PANGOCAIRO_LIBRARIES}
    ${SDL2_LIBRARIES}
)
