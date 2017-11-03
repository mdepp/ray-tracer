#pragma once
#include "vec.h"

#ifdef ARDUINO_BUILD
    #include <Arduino.h>
    #undef abs
    #undef min
    #undef max
    template <typename S, typename T>
    auto inline max(S&& a, T&& b)
    {
        return a < b ? b : a;
    }
    template <typename S, typename T>
    auto inline min(S&& a, T&& b)
    {
        return a < b ? b : a;
    }
    template <typename T>
    auto inline abs(S&& n)
    {
        return n < 0 ? -n : n;
    }

    template<typename T>
    inline void debugPrint(T val)
    {
        Serial.println(val);
    }
#else
    #include <algorithm>
    #include <cmath>
    #include <cstdint>
    #include <iostream>

    using std::abs;
    using std::min;
    using std::max;

    template<typename T>
    inline void debugPrint(T val)
    {
        std::cout << val << std::endl;
    }
    
#endif

template<typename T>
T pow2(T base)
{
    return base*base;
}

uint16_t encodeColour(vec3<> colour);