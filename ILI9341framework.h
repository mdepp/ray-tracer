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
    uint16_t encodeColour(fvec3 colour);

    fvec3 m_backgroundColour;

    const uint16_t m_width;
    const uint16_t m_height;

    Adafruit_ILI9341 m_tft;
};
