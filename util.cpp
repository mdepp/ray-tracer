#include "util.h"

uint16_t encodeColour(glm::vec3 colour)
{
    uint16_t r = colour.r * 32;
    uint16_t g = colour.g * 64;
    uint16_t b = colour.b * 32;

    return (r << 11) + (g << 5) + b;
}