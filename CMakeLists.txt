cmake_minimum_required(VERSION 3.20)
project(woof VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB SOURCES src/*.cpp)

add_executable(woof ${SOURCES})
target_compile_options(woof PRIVATE
    "-sUSE_SDL=2"
)

message( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ARUFF sdl2=$ENV{SDL_ROOT}" )

target_link_options(woof PRIVATE
    "--preload-file ${CMAKE_SOURCE_DIR}/res"
    "-sUSE_SDL=2"
)
