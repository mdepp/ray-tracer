#ifdef ARDUINO_BUILD

#include "ILI9341framework.h"

#include "util.h"

ILI9341Framework::ILI9341Framework()
 : m_backgroundColour(0.f, 0.f, 0.f), m_width(320), m_height(240), m_tft(10, 9)
{
    // Initialize general Arduino-specific libraries
    init();
    Serial.begin(9600);
    // Initialize screen and rotate to the standard orientation used in class.
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
bool ILI9341Framework::idle()
{
    delay(10000000);
    return true;
}
void ILI9341Framework::clear(fvec3 colour)
{
    m_backgroundColour = colour;
    m_tft.fillScreen(encodeColour(m_backgroundColour));
}
void ILI9341Framework::drawPixel(uint16_t x, uint16_t y, fvec3 colour)
{
    uint16_t encoded = encodeColour(colour);
    if (m_backgroundColour != colour)
    {
        // Don't draw pixels of background colour (since drawing is really slow)
        m_tft.drawPixel(x, m_height-1-y, encodeColour(colour));
    }
    // Send pixel colour to save_image.py
    Serial.write((char)2);
    Serial.write(lowByte(encoded));
    Serial.write(highByte(encoded));
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
    uint16_t r = constrain(colour.r * 31, 0, 31);
    uint16_t g = constrain(colour.g * 63, 0, 63);
    uint16_t b = constrain(colour.b * 31, 0, 31);
    return (r << 11) + (g << 5) + b;
}

#endif
