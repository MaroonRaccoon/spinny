
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL.h>
#include <SDL_opengles2.h>
#endif

#include <iostream>
#include "graphics.hpp"

void mainLoop(SDL_Window *window) 
{
    gfx::render(window);
}

void mainLoopWrapper(void* window)
{
    mainLoop(static_cast<SDL_Window *>(window));
}

SDL_Window *initWindow()
{
    int winWidth = 512, winHeight = 512;

    // Create SDL window
    SDL_Window *window = 
        SDL_CreateWindow("Hello Triangle Minimal", 
                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         winWidth, winHeight, 
                         SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    // Create OpenGLES 2 context on SDL window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GLContext glc = SDL_GL_CreateContext(window);
    std::cout << "INFO: GL version: " << glGetString(GL_VERSION) << std::endl;

    // Set clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Get actual GL window size in pixels, in case of high dpi scaling
    SDL_GL_GetDrawableSize(window, &winWidth, &winHeight);
    std::cout << "INFO: GL window size = " << winWidth << "x" << winHeight << std::endl;
    glViewport(0, 0, winWidth, winHeight);   
    return window;
}

int main(int argc, char** argv)
{
    SDL_Window *window = initWindow();
    GLuint shaderProgram = gfx::initShader();
    gfx::initGeometry(shaderProgram);

    int fps = 0; // Use browser's requestAnimationFrame
    emscripten_set_main_loop_arg(mainLoopWrapper, window, fps, true);

    return 0;
}
