cmake_minimum_required(VERSION 3.20)
project(woof VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE on)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/out)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB SOURCES ${SRC_DIR}/*.cpp)

add_executable(woof ${SOURCES})
add_custom_command(
    OUTPUT woof.html
    COMMAND ${CMAKE_COMMAND} -E copy ${SRC_DIR}/woof.html ${OUTPUT_DIR}/woof.html
    DEPENDS ${SRC_DIR}/woof.html
)
add_custom_target(
    woof_html
    DEPENDS woof.html
)
add_dependencies(woof woof_html)

set_target_properties(woof
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)
target_compile_options(woof PRIVATE
    "-sUSE_SDL=2"
)
target_link_options(woof PRIVATE
    "--embed-file ${CMAKE_SOURCE_DIR}/res/shaders@/hello_triangle.vert"
    "--embed-file ${CMAKE_SOURCE_DIR}/res/shaders@/hello_triangle.frag"
    "--embed-file somefile.txt"
    "-sNO_DISABLE_EXCEPTION_CATCHING"
    "-sUSE_SDL=2"
)
