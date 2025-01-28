#!/bin/sh

cmake -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN_ROOT/share/emscripten/cmake/Modules/Platform/Emscripten.cmake \
      -DCMAKE_BUILD_TYPE=Debug \
      -G "Unix Makefiles" \
      ..
