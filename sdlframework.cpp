#ifndef ARDUINO_BUILD

#include "sdlframework.h"
#include "util.h"
#include "vec.h"

SDLFramework::SDLFramework()
 : m_width(320), m_height(240),
    m_updatePeriod(1000)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        util::debugPrint("Cannot initialize SDL");
        return;
    }

    if (SDL_CreateWindowAndRenderer(m_width, m_height, 0, &m_window, &m_renderer))
    {
        util::debugPrint("Cannot create SDL window and renderer.");
        return;
    }

    m_lastUpdate = SDL_GetTicks();
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

    auto currentTime = SDL_GetTicks();
    auto elapsedTime = currentTime - m_lastUpdate;
    if (elapsedTime >= m_updatePeriod)
    {
        SDL_RenderPresent(m_renderer);
        m_lastUpdate = currentTime;
    }

    return true;
}

void SDLFramework::clear(vec3<> colour)
{
    m_backgroundColour = colour;
    SDL_SetRenderDrawColor(m_renderer, colour.r*0xFF, colour.g*0xFF, colour.b*0xFF, 0xFF);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);
}

void SDLFramework::drawPixel(uint16_t x, uint16_t y, vec3<> colour)
{
    if (colour == m_backgroundColour) return;
    SDL_SetRenderDrawColor(m_renderer, colour.r*0xFF, colour.g*0xFF, colour.b*0xFF, 0xFF);
    SDL_RenderDrawPoint(m_renderer, x, y);
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