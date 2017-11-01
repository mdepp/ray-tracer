#pragma once
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

uint16_t encodeColour(glm::vec3 colour);

template<typename T>
void debugPrint(T val)
{
    std::cout << val << std::endl;
}