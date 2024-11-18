#pragma once

#include "SFML/Graphics/Rect.hpp"
#include "geometry/alignment.h"
#include "point.h"
#include "size.h"

#include <algorithm>

template <typename T> struct Rect
{
    Rect() : pos{ 0.f, 0.f }, size{ 0.f, 0.f }
    {
    }
    explicit Rect(Point<T> pos, Size<T> size) : pos{ pos }, size{ size }
    {
    }
    explicit Rect(T x, T y, T width, T height) : pos{ x, y }, size{ width, height }
    {
    }

    T width() const
    {
        return size.width;
    }
    T height() const
    {
        return size.height;
    }

    Point<T> topLeft() const
    {
        return pos;
    }
    Point<T> topRight() const
    {
        return { pos.x() + width(), pos.y() };
    }
    Point<T> bottomLeft() const
    {
        return { pos.x(), pos.y() + height() };
    }
    Point<T> bottomRight() const
    {
        return { pos.x + width(), pos.y + height() };
    }
    Point<T> center() const
    {
        return { pos.x() + width() / 2, pos.y() + height() / 2 };
    }

    Point<T> pointBy(Align origin) const
    {
        Point<T> result{ pos.x() + width() / 2, pos.y() + height() / 2 };

        double halfWidth{ width() / 2 };
        double halfHeight{ height() / 2 };

        size_t value{ static_cast<size_t>(origin) };

        if (value & static_cast<size_t>(Align::Left))
            result.moveX(-halfWidth);
        if (value & static_cast<size_t>(Align::Right))
            result.moveX(halfWidth);
        if (value & static_cast<size_t>(Align::Top))
            result.moveY(-halfHeight);
        if (value & static_cast<size_t>(Align::Bottom))
            result.moveY(halfHeight);

        return result;
    }

    const Rect<T> &operator|(const Rect<T> &rect)
    {
        return { { std::max(pos.x(), rect.pos.x()), std::max(pos.y(), rect.pos.y()) },
                 { std::max(size.width, rect.size.width),
                   std::max(size.height, rect.size.height) } };
    }

    const Rect<T> &operator|=(const Rect<T> &rect)
    {
        pos = { std::max(pos.x(), rect.pos.x()), std::max(pos.y(), rect.pos.y()) };
        size = { std::max(size.width, rect.size.width), std::max(size.height, rect.size.height) };

        return *this;
    }

    operator Rect<float>() const
    {
        return Rect<float>{ { static_cast<float>(pos.x()), static_cast<float>(pos.y()) },
                            { static_cast<float>(size.width), static_cast<float>(size.height) } };
    }

    Point<T> pos;
    Size<T> size;
};

using RectF = Rect<float>;
using RectU = Rect<size_t>;
