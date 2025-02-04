#pragma once

#ifdef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_opengles2.h>
#include <emscripten.h>
#endif

namespace gfx
{

GLuint initShader();
void   initGeometry( GLuint shaderProgram );
void   render( SDL_Window *window );

} // namespace gfx
