# Copyright 2014 Jon Eyolfson

include_directories(
    ${LNCHR_SOURCE_DIR}/include
    ${PANGOCAIRO_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

configure_file(version.cpp.in version.cpp)

add_executable(lnchr lnchr.cpp ${LNCHR_BINARY_DIR}/src/version.cpp)
target_link_libraries(lnchr
    ${PANGOCAIRO_LIBRARIES}
    ${SDL2_LIBRARIES}
)
