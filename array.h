#pragma once

#include "util.h"

template <typename T, size_t MAX_SIZE>
class Array
{
public:
    using Type = T;

    template <typename... Args>
    Array(Args&&... args) : m_data{util::forward<Args>(args)...}
    {
        m_curSize = sizeof...(Args);
    }
    Array()
    {
        m_curSize = 0;
    }

    template <typename... Args>
    bool add(Args&&... args)
    {
        if (m_curSize == MAX_SIZE)
            return false;
        m_data[m_curSize++] = T(util::forward<Args>(args)...);
        return true;
    }
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
    T* begin()
    {
        return m_data;
    }
    T* end()
    {
        return m_data+m_curSize;
    }
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
