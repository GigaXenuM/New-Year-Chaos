#pragma once

#include <utility>

#include "SFML/System/Vector2.hpp"

template <typename T> struct Point
{
    Point();
    Point(T x, T y);

    Point operator+(const Point &other) const;
    Point operator-(const Point &other) const;
    Point operator-() const;
    Point &operator+=(const Point &other) const;
    Point &operator-=(const Point &other) const;
    Point operator/(T value) const;
    Point operator*(T value) const;
    bool operator==(const Point &other) const;
    bool operator!=(const Point &other) const;

    bool isDefault() const;

    T x() const;
    T y() const;

    void setX(T x);
    void setY(T y);
    void moveX(T offset);
    void moveY(T offset);

    T dot(const Point &other) const;
    Point normal() const;
    bool overlap(const Point &other) const;
    T overlapLength(const Point &other) const;

    sf::Vector2<T> data() const;

private:
    sf::Vector2<T> _data;
};

using PointF = Point<float>;
using PointI = Point<int>;

using VectorF = Point<float>;

template <typename T> Point<T>::Point() : _data{}
{
}

template <typename T> Point<T>::Point(T x, T y) : _data{ x, y }
{
}

template <typename T> Point<T> Point<T>::operator+(const Point &other) const
{
    return { x() + other.x(), y() + other.y() };
}

template <typename T> Point<T> Point<T>::operator-(const Point &other) const
{
    return { x() - other.x(), y() - other.y() };
}

template <typename T> Point<T> Point<T>::operator-() const
{
    return { -x(), -y() };
}

template <typename T> Point<T> &Point<T>::operator+=(const Point &other) const
{
    return *this = *this + other;
}

template <typename T> Point<T> &Point<T>::operator-=(const Point &other) const
{
    return *this = *this - other;
}

template <typename T> Point<T> Point<T>::operator/(T value) const
{
    return { x() / value, y() / value };
}

template <typename T> Point<T> Point<T>::operator*(T value) const
{
    return { x() * value, y() * value };
}

template <typename T> bool Point<T>::operator==(const Point &other) const
{
    return x() == other.x() && y() == other.y();
}

template <typename T> bool Point<T>::operator!=(const Point &other) const
{
    return !(*this == other);
}

template <typename T> bool Point<T>::isDefault() const
{
    return x() == T() && y() == T();
}

template <typename T> T Point<T>::x() const
{
    return _data.x;
}

template <typename T> T Point<T>::y() const
{
    return _data.y;
}

template <typename T> void Point<T>::setX(T x)
{
    _data.x = x;
}

template <typename T> void Point<T>::setY(T y)
{
    _data.y = y;
}

template <typename T> void Point<T>::moveX(T offset)
{
    _data.x += offset;
}

template <typename T> void Point<T>::moveY(T offset)
{
    _data.y += offset;
}

template <typename T> T Point<T>::dot(const Point &other) const
{
    return _data.x * other._data.x + _data.y * other._data.y;
}

template <typename T> Point<T> Point<T>::normal() const
{
    sf::Vector2<T> normal{ _data.normalized().perpendicular() };
    return { normal.x, normal.y };
}

template <typename T> bool Point<T>::overlap(const Point &other) const
{
    return x() <= other.y() && y() >= other.x();
}

template <typename T> T Point<T>::overlapLength(const Point &other) const
{
    if (!overlap(other))
        return 0.f;

    return std::min(y(), other.y()) - std::max(x(), other.x());
}

template <typename T> sf::Vector2<T> Point<T>::data() const
{
    return _data;
}
