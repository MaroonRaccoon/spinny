
#ifdef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_opengles2.h>
#include <emscripten.h>
#include <glm/glm.hpp>
#endif

#include <iostream>

#include "graphics.hpp"
#include "window.hpp"

void mainLoop( gfx::Window *window )
{
    gfx::render( window );
}

void mainLoopWrapper( void *window )
{
    mainLoop( static_cast< gfx::Window * >( window ) );
}

int main( int argc, char **argv )
{
    gfx::Dimensions windowSize( 512, 512 );
    gfx::Window     window( "woof!", windowSize.width, windowSize.height );

    // Create OpenGLES 2 context on SDL window
    SDL_GLContext glc = window.createContext();
    std::cout << "INFO: GL version: " << glGetString( GL_VERSION ) << std::endl;

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    // Get actual GL window size in pixels, in case of high dpi scaling
    windowSize = window.getDrawableSize();
    std::cout << "INFO: GL window size = " << windowSize.width << "x" << windowSize.height << std::endl;
    glViewport( 0, 0, windowSize.width, windowSize.height );

    GLuint shaderProgram = gfx::initShader();
    gfx::initGeometry( shaderProgram );

    int fps = 0; // Use browser's requestAnimationFrame
    emscripten_set_main_loop_arg( mainLoopWrapper, &window, fps, true );

    return 0;
}
