#include <algorithm>
#include <cmath> // Change to <Arduino.h>
#include <cstdint>
#include <iostream>
#include <glm/glm.hpp>

using std::abs;
using std::min;
using std::max;

template<typename T>
T pow2(T base)
{
    return base*base;
}

uint16_t encodeColour(glm::vec3 colour)
{
    uint16_t r = colour.r*32;
    uint16_t g = colour.g*64;
    uint16_t b = colour.b*32;

    return (r<<11) + (g<<5) + b;
}

template<typename T>
void debugPrint(T val)
{
    std::cout << val << std::endl;
}