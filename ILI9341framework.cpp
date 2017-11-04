#ifdef ARDUINO_BUILD

#include "ILI9341framework.h"

#include "util.h"

ILI9341Framework::ILI9341Framework()
 : m_width(320), m_height(240), m_tft(10, 9)
{
    init();
    Serial.begin(9600);
    m_tft.begin();
    m_tft.setRotation(3);
}
ILI9341Framework::~ILI9341Framework()
{
}
bool ILI9341Framework::tick()
{
    return true; // Can't 'close' a ILI9341 screen
}
void ILI9341Framework::clear(vec3<> colour)
{
    m_tft.fillScreen(encodeColour(colour));
}
void ILI9341Framework::drawPixel(uint16_t x, uint16_t y, vec3<> colour)
{
    m_tft.drawPixel(x, y, ILI9341_WHITE);
}
uint16_t ILI9341Framework::width()
{
    return m_width;
}
uint16_t ILI9341Framework::height()
{
    return m_height;
}
uint16_t ILI9341Framework::encodeColour(vec3<float> colour)
{
    uint16_t r = colour.r * 32;
    uint16_t g = colour.g * 64;
    uint16_t b = colour.b * 32;

    return (r << 11) + (g << 5) + b;
}

#endif