cmake_minimum_required(VERSION 3.20)
project(spinny_app VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE on)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/out)
set(BUILD_DIR ${CMAKE_BINARY_DIR})
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${PROJECT_ROOT_DIR}/src)

# ---- main html target ----
add_custom_command(
    OUTPUT spinny.html
    COMMAND ${CMAKE_COMMAND} -E copy ${SRC_DIR}/spinny.html ${OUTPUT_DIR}/spinny.html
    DEPENDS ${SRC_DIR}/spinny.html
)
#add_custom_target(
#    spinny_html ALL # ALL: makes the ALL target depend on spinny_html to ensure spinny_html is built
#    DEPENDS spinny.html
#)

# ---- main wasm & js target  ----
add_executable(spinny
    ${SRC_DIR}/spinny.cpp
    ${SRC_DIR}/graphics.cpp
    ${SRC_DIR}/window.cpp
)
set_target_properties(spinny
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)
set_property(TARGET spinny 
    PROPERTY LINK_DEPENDS
        ${CMAKE_SOURCE_DIR}/res/shaders/hello_triangle.vert
        ${CMAKE_SOURCE_DIR}/res/shaders/hello_triangle.frag
)

target_include_directories(spinny PRIVATE
    $ENV{GLM_PATH}/include
)

target_compile_options(spinny PRIVATE
    "-sUSE_SDL=2"
)
target_link_options(spinny PRIVATE
    --embed-file ${CMAKE_SOURCE_DIR}/res@res
    -sNO_DISABLE_EXCEPTION_CATCHING
    -sUSE_SDL=2
)

# ---- spinny target ----
add_custom_target(spinny_app ALL DEPENDS spinny spinny.html)

# ---- fix the compile commands from em++ ----
set(CLANGD_FILE ${CMAKE_BINARY_DIR}/clangd)
if(EMSCRIPTEN)
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER}
            -sUSE_SDL=2
            --cflags 
        OUTPUT_VARIABLE CLANGD_FLAGS_TO_ADD
    )
    separate_arguments(CLANGD_FLAGS_TO_ADD UNIX_COMMAND "${CLANGD_FLAGS_TO_ADD}")
    list(JOIN CLANGD_FLAGS_TO_ADD ", " CLANGD_FLAGS_TO_ADD)
    set(CLANGD_TEMPLATE ${PROJECT_SOURCE_DIR}/scripts/clangd.in)
    set(CLANGD_FLAGS_TO_REMOVE -sUSE_SDL*)
    configure_file(${CLANGD_TEMPLATE} ${CLANGD_FILE})
else()
    file(WRITE ${CLANGD_FILE} "")
endif()

# ---- clangd compile commands & config target ----
add_custom_target(spinny_clangd
    COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/clangd ${PROJECT_ROOT_DIR}/.clangd
    COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/compile_commands.json ${PROJECT_ROOT_DIR}/compile_commands.json
    DEPENDS spinny_app
)
add_custom_target(develop DEPENDS spinny_clangd)
