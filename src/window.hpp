#pragma once

#ifdef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_opengles2.h>
#include <emscripten.h>
#endif

#include <string>

namespace gfx
{

struct Dimensions
{
    int width;
    int height;
};

class Window
{
  public:
    Window( std::string title, int width, int height );
    ~Window();
    void          swap();
    SDL_GLContext createContext();
    Dimensions    getDrawableSize();

  private:
    SDL_Window *window;
};

} // namespace gfx
