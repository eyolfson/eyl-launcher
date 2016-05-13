# Copyright 2014 Jon Eyolfson
#
# This file is distributed under the GPLv3 license

include_directories(
    ${LNCHR_SOURCE_DIR}/include
    ${PANGOCAIRO_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

configure_file(version.cpp.in version.cpp)

add_executable(eyl-launcher
    main.cpp
    xdg.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
)

target_link_libraries(eyl-launcher
    ${PANGOCAIRO_LIBRARIES}
    ${SDL2_LIBRARIES}
)
