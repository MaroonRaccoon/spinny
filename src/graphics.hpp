#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL.h>
#include <SDL_opengles2.h>
#endif


namespace gfx {

GLuint initShader();
void initGeometry(GLuint shaderProgram);
void render(SDL_Window *window);

}
