/*
 * File: ILI9341framework.h
 * Authors: Alexander Epp (1487716) and Mitchell Epp (1498821)
 * Project: CMPUT274 Final Project
 * Description: Implement the WindowFramework interface for
 *              desktop machines using SDL2. This updates the
 *              screen every second (via tick() or idle()).
 */

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
    /*
     * Update pixels displayed to screen with pixels drawn on back buffer with
     * drawPixel. 
     */
    void paintBackBuffer();
    void lockBackBuffer();
    void unlockBackBuffer();
    
    /*
     * Convert a colour vector to the format used by the backbuffer
     */
    uint32_t encodeColour(fvec3 colour);

    // Dimensions of window in pixels
    const uint16_t m_width;
    const uint16_t m_height;

    const uint32_t m_updatePeriod; // Period to wait before updating the screen (in milliseconds)
    uint32_t m_lastUpdate; // Time (ms) that screen was last updated

    // Back buffer information
    int m_backBufferPitch;
    uint32_t* m_backBufferPixels;
    SDL_Texture* m_backBuffer;
    // Various SDL structs
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Event m_event;
};
