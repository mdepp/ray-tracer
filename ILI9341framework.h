#pragma once

#include "windowframework.h"

#include <Adafruit_ILI9341.h>

class ILI9341Framework : public WindowFramework
{
public:
    ILI9341Framework();
    virtual ~ILI9341Framework();

    bool tick() override;

    void clear(vec3<> colour) override;
    void drawPixel(uint16_t x, uint16_t y, vec3<> colour) override;

    uint16_t width() override;
    uint16_t height() override;

private:
    uint16_t encodeColour(vec3<> colour);
    
    vec3<> m_backgroundColour;

    const uint16_t m_width;
    const uint16_t m_height;

    Adafruit_ILI9341 m_tft;
};