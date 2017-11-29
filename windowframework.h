#pragma once
#include "util.h"
#include "vec.h"

class WindowFramework
{
public:
    WindowFramework();

    /*
     * Update framework; return false if should quit.
     */
    virtual bool tick() = 0;
    /*
     * Idles -- returns false if should quit
     */
    virtual bool idle() = 0;

    virtual void clear(fvec3 colour) = 0;
    /*
     * Draw a single pixel to the screen (x, y are standard Cartesian coordinates, e.i.,
     * x ranges from 0 (left) to width (right)
     * y ranges from 0 (bottom) to height (top)
     */
    virtual void drawPixel(uint16_t x, uint16_t y, fvec3 colour) = 0;

    virtual uint16_t width() = 0;
    virtual uint16_t height() = 0;

    virtual ~WindowFramework();
};
