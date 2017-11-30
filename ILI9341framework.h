/*
 * This implements the WindowFramework interface for Arduino, using the Adafruit
 * ILI9341 display library. It is assumed that the screen is wired as in class;
 * in particular, it expects CS <--> 10 and D/C <--> 9.
 * 
 * This class also initializes all Arduino functions, and the Arduino Serial
 * library, which are needed for utility functions on the Arduino.
 */

#pragma once

#include "windowframework.h"
#include <Adafruit_ILI9341.h>

class ILI9341Framework : public WindowFramework
{
public:
    ILI9341Framework();
    virtual ~ILI9341Framework();

    virtual bool tick() override;
    virtual bool idle() override;

    virtual void clear(fvec3 colour) override;
    virtual void drawPixel(uint16_t x, uint16_t y, fvec3 colour) override;

    virtual uint16_t width() override;
    virtual uint16_t height() override;

private:
    // Convert a colour vector to the format used by the ILI9341 library
    uint16_t encodeColour(fvec3 colour);

    // Stored to prevent drawing pixels of this colour, as an optimization
    fvec3 m_backgroundColour;

    // Dimensions of the screen in pixels
    const uint16_t m_width;
    const uint16_t m_height;

    Adafruit_ILI9341 m_tft;
};
