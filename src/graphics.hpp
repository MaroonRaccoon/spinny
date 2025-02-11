#pragma once

#ifdef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_opengles2.h>
#include <emscripten.h>
#endif

#include "window.hpp"
#include "game.hpp"

namespace gfx
{

GLuint initShader();
void   initGeometry( GLuint shaderProgram );
void   render( Game &game, gfx::Window &window );

} // namespace gfx
