#pragma once
#include "windowframework.h"
#include "util.h"
#include "vec.h"
#include <SDL2/SDL.h>

class SDLFramework : public WindowFramework
{
public:
    SDLFramework();
    virtual ~SDLFramework();

    virtual bool tick() override;
    virtual bool idle() override;

    virtual void clear(fvec3 colour) override;
    virtual void drawPixel(uint16_t x, uint16_t y, fvec3 colour) override;

    virtual uint16_t width() override;
    virtual uint16_t height() override;

private:
    void paintBackBuffer();
    void lockBackBuffer();
    void unlockBackBuffer();

    const uint16_t m_width;
    const uint16_t m_height;

    const uint32_t m_updatePeriod; // Period to wait before updating the screen (in milliseconds)
    uint32_t m_lastUpdate; // Time (ms) that screen was last updated

    fvec3 m_backgroundColour;

    int m_backBufferPitch;
    uint32_t* m_backBufferPixels;
    SDL_Texture* m_backBuffer;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Event m_event;
};
