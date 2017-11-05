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

    bool tick() override;
    
    void clear(vec3<> colour) override;
    void drawPixel(uint16_t x, uint16_t y, vec3<> colour) override;

    uint16_t width() override;
    uint16_t height() override;

private:
    const uint16_t m_width;
    const uint16_t m_height;

    const uint32_t m_updatePeriod; // Period to wait before updating the screen (in milliseconds)
    uint32_t m_lastUpdate; // Time (ms) that screen was last updated

    vec3<> m_backgroundColour;

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Event m_event;
};