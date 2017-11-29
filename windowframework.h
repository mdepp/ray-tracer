/*
 * This file contains the WindowFramework interface, which is used to
 * standardize drawing to the screen across the Arduino and desktop platforms.
 */

#pragma once
#include "util.h"
#include "vec.h"

class WindowFramework
{
public:
    WindowFramework();

    /*
     * Update framework; return false if should quit. This function should be
     * called regularily when drawing or otherwise not idling, since it
     * will eventually show drawn pixels on the screen (although not necessarily
     * in a real-time manner), which drawPixel() is not guarenteed to do.
     */
    virtual bool tick() = 0;
    /*
     * Idles -- returns false if should quit.
     */
    virtual bool idle() = 0;
    
    /*
     * Clear all screen pixels to a given colour.
     */
    virtual void clear(fvec3 colour) = 0;
    /*
     * Draw a single pixel to the screen (x, y are standard Cartesian coordinates, e.i.,
     * x ranges from 0 (left) to width (right)
     * y ranges from 0 (bottom) to height (top).
     * The pixel is not guarenteed to be drawn immediately (although it might be
     * for certain implementations), and so tick() should be called regularily
     * when drawing pixels.
     */
    virtual void drawPixel(uint16_t x, uint16_t y, fvec3 colour) = 0;

    virtual uint16_t width() = 0;
    virtual uint16_t height() = 0;

    virtual ~WindowFramework();
};
