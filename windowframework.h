#pragma once
#include "utils.h"

class WindowFramework
{
public:
    WindowFramework();

    /*
     * Update framework; return false if should quit.
     */
    virtual bool tick() = 0;

    virtual void clear(glm::vec3 colour) = 0;
    virtual void drawPixel(uint16_t x, uint16_t y, glm::vec3 colour) = 0;

    virtual uint16_t width() = 0;
    virtual uint16_t height() = 0;

    virtual ~WindowFramework();
};