#ifndef ARDUINO_BUILD

#include "sdlframework.h"
#include "util.h"
#include "vec.h"

SDLFramework::SDLFramework()
 : m_width(320), m_height(240)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        debugPrint("Cannot initialize SDL");
        return;
    }

    if (SDL_CreateWindowAndRenderer(m_width, m_height, 0, &m_window, &m_renderer))
    {
        debugPrint("Cannot create SDL window and renderer.");
        return;
    }
}
SDLFramework::~SDLFramework()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool SDLFramework::tick()
{
    while (SDL_PollEvent(&m_event))
    {
        if (m_event.type == SDL_QUIT) return false;
        if (m_event.type == SDL_KEYUP && m_event.key.keysym.sym == SDLK_ESCAPE) return false;
    }
    return true;
}

void SDLFramework::clear(vec3<> colour)
{
    SDL_SetRenderDrawColor(m_renderer, colour.r*0xFF, colour.g*0xFF, colour.b*0xFF, 0xFF);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);
}

void SDLFramework::drawPixel(uint16_t x, uint16_t y, vec3<> colour)
{
    SDL_SetRenderDrawColor(m_renderer, colour.r*0xFF, colour.g*0xFF, colour.b*0xFF, 0xFF);
    SDL_RenderDrawPoint(m_renderer, x, y);
    SDL_RenderPresent(m_renderer);
}

uint16_t SDLFramework::width()
{
    return m_width;
}
uint16_t SDLFramework::height()
{
    return m_height;
}

#endif