#include "window.hpp"
#include <iostream>

namespace gfx
{

Window::Window( std::string title, int width, int height )
    : window( SDL_CreateWindow(
          title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
      ) )
{
}

Window::~Window()
{
    SDL_DestroyWindow( window );
}

void Window::swap()
{
    SDL_GL_SwapWindow( window );
}

Dimensions Window::getDrawableSize()
{
    Dimensions size( 0, 0 );
    SDL_GL_GetDrawableSize( window, &size.width, &size.height );
    return size;
}

SDL_GLContext Window::createContext()
{
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
    SDL_GL_SetSwapInterval( 1 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
    return SDL_GL_CreateContext( window );
}

} // namespace gfx
