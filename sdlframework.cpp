#ifndef ARDUINO_BUILD

#include "sdlframework.h"
#include "util.h"
#include "vec.h"

SDLFramework::SDLFramework()
//   : m_width(320), m_height(240),
   : m_width(1920), m_height(1080),
//   : m_width(1920/2), m_height(1080/2),
    m_updatePeriod(1000)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        util::debugPrint("Cannot initialize SDL");
        return;
    }

    uint32_t windowFlags = 0;

    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        util::debugPrint("SDL display mode query failed");
        return;
    }
    if (dm.w == m_width && dm.h == m_height)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    if (SDL_CreateWindowAndRenderer(m_width, m_height, windowFlags, &m_window, &m_renderer))
    {
        util::debugPrint("Cannot create SDL window and renderer.");
        return;
    }

    m_backBuffer = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        m_width,
        m_height);
    lockBackBuffer();

    m_lastUpdate = SDL_GetTicks();
}
SDLFramework::~SDLFramework()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void SDLFramework::paintBackBuffer()
{
    unlockBackBuffer();
    SDL_RenderCopy(m_renderer, m_backBuffer, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
    lockBackBuffer();
}

void SDLFramework::unlockBackBuffer()
{
    SDL_UnlockTexture(m_backBuffer);
}

void SDLFramework::lockBackBuffer()
{
    SDL_LockTexture(m_backBuffer, nullptr, (void**)&m_backBufferPixels, &m_backBufferPitch);
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
        paintBackBuffer();
        m_lastUpdate = currentTime;
    }

    return true;
}

bool SDLFramework::idle()
{
    paintBackBuffer();
    if (SDL_WaitEvent(&m_event) == 0)
    {
        util::debugPrint("SDL_WaitEvent failed.");
        return false;
    }
    if (m_event.type == SDL_QUIT) return false;
    if (m_event.type == SDL_KEYUP && m_event.key.keysym.sym == SDLK_ESCAPE) return false;
    return true;
}

void SDLFramework::clear(fvec3 colour)
{
    m_backgroundColour = colour;
    int r = colour.r * 0xFF;
    int g = colour.g * 0xFF;
    int b = colour.b * 0xFF;
    auto argb = 0xFF000000 | (r<<16) | (g<<8) | b;
    for (int i = 0; i < m_width*m_height; ++i)
        *(m_backBufferPixels+i) = argb;
    paintBackBuffer();
}

void SDLFramework::drawPixel(uint16_t x, uint16_t y, fvec3 colour)
{
    if (colour == m_backgroundColour) return;
    //SDL_SetRenderDrawColor(m_renderer, colour.r*0xFF, colour.g*0xFF, colour.b*0xFF, 0xFF);
    //SDL_RenderDrawPoint(m_renderer, x, y);

    int r = util::min(colour.r * 0xFF, 255.f);
    int g = util::min(colour.g * 0xFF, 255.f);
    int b = util::min(colour.b * 0xFF, 255.f);
    *(m_backBufferPixels+x + y*m_width) = 0xFF000000 | (r<<16) | (g<<8) | b;
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
