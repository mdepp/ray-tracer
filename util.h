#pragma once


#ifdef ARDUINO_BUILD
#include <Arduino.h>
#undef abs
#undef min
#undef max
namespace util
{
    template<typename T>
    T max(T a, T b) {return a < b ? b : a;}
    template<typename T>
    T min(T a, T b) {return a < b ? a : b;}
    template<typename T>
    T abs(T n) {return n < 0 ? -n : n;}
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
    // Generate a random integer in the range [0, max)
    int32_t random(int32_t max);

    template<typename T, int Size>
    T getSeed(int pin, int delay_time)
    {
      T val = 0;
      for(int i = 0; i < Size; ++i)
      {
          // Get the least significant bit of the random input
          T random_digit = analogRead(pin) & 1;
          // Set this as the i'th bit of val
          val += random_digit << i;
          // Delay a bit for input value to change
          delay(delay_time);
      }
      return val;
    }

    // Seed random number generator nondeterministically
    void srand();
    // Seed random number generator
    void srand(uint32_t);

    uint8_t* getStackPointer();
    // https://github.com/esp8266/Arduino/issues/81
    uint32_t getAvailableMemory();
}
#else
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
namespace util
{
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

    // Generate a random integer in the range [0, max)
    int32_t random(int32_t maxval);
    // Seed random number generator nondeterministically
    void srand();
    // Seed random number generator
    void srand(uint32_t);

    uint8_t* getStackPointer();
    // https://github.com/esp8266/Arduino/issues/81
    uint32_t getAvailableMemory();
}
#endif

namespace util
{
    template<typename T>
    T pow2(T base)
    {
        return base*base;
    }

    // http://en.cppreference.com/w/cpp/types/remove_reference
    template< class T > struct remove_reference { typedef T type; };
    template< class T > struct remove_reference<T&> { typedef T type; };
    template< class T > struct remove_reference<T&&> { typedef T type; };

    // http://en.cppreference.com/w/cpp/types/integral_constant
    template<class T, T v>
    struct integral_constant {
        static constexpr T value = v;
        typedef T value_type;
        typedef integral_constant type; // using injected-class-name
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; } //since c++14
    };
    template <bool B>
    using bool_constant = integral_constant<bool, B>;

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;


    // http://en.cppreference.com/w/cpp/types/is_lvalue_reference
    template<class T> struct is_lvalue_reference : false_type {};
    template<class T> struct is_lvalue_reference<T&> : true_type {};

    // https://stackoverflow.com/a/27501759
    template <class T>
    constexpr inline T&& forward(typename remove_reference<T>::type& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template <class T>
    constexpr inline T&& forward(typename remove_reference<T>::type&& t) noexcept
    {
        static_assert(!is_lvalue_reference<T>::value,
            "Can not forward an rvalue as an lvalue.");
        return static_cast<T&&>(t);
    }
}
