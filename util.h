#pragma once


#ifdef ARDUINO_BUILD
#include <Arduino.h>
#undef abs
#undef min
#undef max
namespace util
{
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
}
#else
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
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

    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;


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