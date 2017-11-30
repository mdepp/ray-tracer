/*
 * Contains implementation of the SDLFramework class from sdlframework.h
 */

#ifndef ARDUINO_BUILD

#include "sdlframework.h"
#include "util.h"
#include "vec.h"

SDLFramework::SDLFramework()
//   : m_width(320), m_height(240),
//   : m_width(1920), m_height(1080),
   : m_width(1920/2), m_height(1080/2),
    m_updatePeriod(1000)
{
    // Initialize SDL
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
    // If a window is being created with dimensions equal to screen dimensions,
    // make that window full screen.
    if (dm.w == m_width && dm.h == m_height)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    // Create and display window
    if (SDL_CreateWindowAndRenderer(m_width, m_height, windowFlags, &m_window, &m_renderer))
    {
        util::debugPrint("Cannot create SDL window and renderer.");
        return;
    }
    // Create back buffer texture
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
    // Process all events that happened since this function was last called
    while (SDL_PollEvent(&m_event))
    {
        if (m_event.type == SDL_QUIT) return false;
        if (m_event.type == SDL_KEYUP && m_event.key.keysym.sym == SDLK_ESCAPE) return false;
    }

    auto currentTime = SDL_GetTicks();
    auto elapsedTime = currentTime - m_lastUpdate;
    if (elapsedTime >= m_updatePeriod)
    {
        // Periodically update screen with pixels from backbuffer.
        paintBackBuffer();
        m_lastUpdate = currentTime;
    }

    return true;
}

bool SDLFramework::idle()
{
    // Draw all remaining pixels in backbuffer
    paintBackBuffer();
    // Wait for events (should probably just loop until quit, re-drawing after
    // each event)
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
    // Clear backbuffer to the given colour
    for (int i = 0; i < m_width*m_height; ++i)
        *(m_backBufferPixels+i) = encodeColour(colour);
    // Update screen screen with backbuffer (not actually guaranteed by the
    // WindowFramework interface, but no harm in doing it here since it is not
    // prohibited either).
    paintBackBuffer();
}

void SDLFramework::encodeColour(fvec3 colour)
{
    int r = util::max(util::min(colour.r * 0xFF, 255.f), 0.f);
    int g = util::max(util::min(colour.g * 0xFF, 255.f), 0.f);
    int b = util::max(util::min(colour.b * 0xFF, 255.f), 0.f);

    return 0xFF000000 | (r<<16) | (g<<8) | b;
}

void SDLFramework::drawPixel(uint16_t x, uint16_t y, fvec3 colour)
{
    // Pixels are normally specified as x+y*width, but in this format y=0 is the
    // top of the screen and y=height-1 is the bottom, and we want the opposite
    // of that.
    *(m_backBufferPixels+x + (m_height-1-y)*m_width) = encodeColour(colour);
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
