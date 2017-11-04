#pragma once


#ifdef ARDUINO_BUILD
    #include <Arduino.h>
    #undef abs
    #undef min
    #undef max
    template <typename S, typename T>
    auto max(S&& a, T&& b)
    {
        return a < b ? b : a;
    }
    template <typename S, typename T>
    auto min(S&& a, T&& b)
    {
        return a < b ? b : a;
    }
    template <typename T>
    auto inline abs(T&& n)
    {
        return n < 0 ? -n : n;
    }

    template<typename T>
    void debugPrint(T&& val)
    {
        Serial.println(val);
    }
    template <typename T, typename... Args>
    void debugPrint(T&& first, Args&&... args)
    {
        Serial.print(first);
        debugPrint(args...);
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
    void debugPrint(T val)
    {
        std::cout << val << std::endl;
    }
    template <typename T, typename... Args>
    void debugPrint(T&& first, Args&&... args)
    {
        std::cout << first;
        debugPrint(std::forward<Args>(args)...);
    }
#endif

template<typename T>
T pow2(T base)
{
    return base*base;
}
