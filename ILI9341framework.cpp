/*
 * File: ILI9341framework.cpp
 * Authors: Alexander Epp, Mitchell Epp
 * Project: CMPUT274 Final Project
 * Description: Implentation of WindowFramework interface that
 *              runs on the Arduino.
 */

#ifdef ARDUINO_BUILD // Only include in build if building for the Arduino

#include "ILI9341framework.h"

#include "util.h"

ILI9341Framework::ILI9341Framework()
 : m_backgroundColour(0.f, 0.f, 0.f), m_width(320), m_height(240), m_tft(10, 9)
{
    // Initialize general Arduino-specific libraries
    init();
    Serial.begin(115200);
    for (int i = 8; i < 64; ++i) Serial.write(0);
    Serial.write((char)3);
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
    m_backgroundColour = colour; // Store for later
    m_tft.fillScreen(encodeColour(m_backgroundColour));
}
void ILI9341Framework::drawPixel(uint16_t x, uint16_t y, fvec3 colour)
{
    uint16_t encoded = encodeColour(colour);
    if (m_backgroundColour != colour) // Only draw if different colour than
                                      // the background colour. This, of course,
                                      // assumes that no pixel is ever written to
                                      // twice.
    {
        // Don't draw pixels of background colour (since drawing is really slow)
        m_tft.drawPixel(x, m_height-1-y, encodeColour(colour));
    }
    // Send pixel to save_image.py

    Serial.write((char)2); // Indicates to listener that this is data,
                           // not debug information
    // Write data one byte at a time, first low, then high
    Serial.write(lowByte(encoded));
    Serial.write(highByte(encoded));
    Serial.flush();
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
    // Bits 15-11 are red, 10-5 are green, 4-0 are blue
    uint16_t r = constrain(colour.r * 31, 0, 31);
    uint16_t g = constrain(colour.g * 63, 0, 63);
    uint16_t b = constrain(colour.b * 31, 0, 31);
    return (r << 11) + (g << 5) + b;
}

#endif
