/*
 * File: array.h
 * Authors: Alexander Epp (1487716) and Mitchell Epp (1498821)
 * Project: CMPUT274 Final Project
 * Description: A static-memory array of a maximum size
 */

#pragma once

#include "util.h"

/*
 * Mimics a raw c-style array (including not using dynamic memory since this
 * should work on the Arduino), but stores a current length and supports
 * range-based for loops. It has two template paramters: the type of data to
 * store, and the maximum size of the array.
 */
template <typename T, size_t MAX_SIZE>
class Array
{
public:
    using Type = T;
    // Initialize the array as empty
    Array() : m_curSize(0)
    {
    }
    // Add an element (like std::vector::emplace_back)
    template <typename... Args>
    bool add(Args&&... args)
    {
        if (m_curSize == MAX_SIZE)
            return false;
        m_data[m_curSize++] = T(util::forward<Args>(args)...);
        return true;
    }
    // Utility functions
    T& front()
    {
        return m_data[0];
    }
    T& back()
    {
        return m_data[m_curSize-1];
    }
    size_t size()
    {
        return m_curSize;
    }
    // Support for range-based for loops
    T* begin()
    {
        return m_data;
    }
    T* end()
    {
        return m_data+m_curSize;
    }
    // Usual array operations
    template <typename Index>
    T& operator [] (Index index)
    {
        return m_data[index];
    }
    T& operator * ()
    {
        return m_data[0];
    }

private:

    size_t m_curSize;
    T m_data[MAX_SIZE];
};
