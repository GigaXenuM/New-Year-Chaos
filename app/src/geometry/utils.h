#pragma once

#include "point.h"
#include "rect.h"
#include "size.h"

#include "SFML/Graphics/Rect.hpp"

namespace Geometry
{

template <typename T> Point<T> toPoint(sf::Vector2<T> point)
{
    return Point<T>{ point.x, point.y };
}

template <typename T> sf::Vector2<T> toSfmlPoint(Point<T> point)
{
    return sf::Vector2<T>{ point.x(), point.y() };
}

template <typename T> Size<T> toSize(sf::Vector2<T> size)
{
    return Size<T>{ size.x, size.y };
}
template <typename T> sf::Vector2<T> toSfmlSize(Size<T> size)
{
    return sf::Vector2<T>{ size.width, size.height };
}
template <typename T> Rect<T> toRect(sf::Rect<T> rect)
{
    return Rect<T>{ toPoint(rect.getPosition()), toSize(rect.getSize()) };
}

template <typename T> sf::Rect<T> toSfmlRect(Rect<T> rect)
{
    return sf::Rect<T>{ toSfmlPoint(rect.pos), toSfmlSize(rect.size) };
}

double distance(PointF p1, PointF p2);
} // namespace Geometry
