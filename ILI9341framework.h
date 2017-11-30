/*
 * File: ILI9341framework.h
 * Authors: Alexander Epp (1487716) and Mitchell Epp (...)
 * Project: CMPUT274 Final Project
 * Description: Implentation of WindowFramework interface that
 *              runs on an Arduino, using AdafruitILI9341.
 */


#pragma once

#include "windowframework.h"

#include <Adafruit_ILI9341.h>

class ILI9341Framework : public WindowFramework
{
    // Public interface should look exactly like WindowFramework
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
    /* Converts colour stored as 3 floats in the range [0, 1]
     * to the uint16_t format required by AdafruitILI9341
     */
    uint16_t encodeColour(fvec3 colour);

    /* Colour the screen has been cleared to. Stored because
     * draw calls are *slow*, and thus it is undesirable to
     * redraw the background colour when unneeded.
     */
    fvec3 m_backgroundColour;

    // Width and height of the screen
    const uint16_t m_width;
    const uint16_t m_height;

    // Adafruit_ILI9341 instance
    Adafruit_ILI9341 m_tft;
};
