#include "roundedrectshape.h"

#include <cmath>

namespace Graphics
{

RoundedRectShape::RoundedRectShape(const sf::Vector2f &size, float radius, std::size_t pointCount)
    : _size(size), _radius(radius), _cornerPointCount(pointCount)
{
    update();
}

void RoundedRectShape::setSize(const sf::Vector2f &size)
{
    _size = size;
    update();
}

const sf::Vector2f &RoundedRectShape::getSize() const
{
    return _size;
}

void RoundedRectShape::setRadius(float radius)
{
    _radius = radius;
    update();
}

float RoundedRectShape::getRadius() const
{
    return _radius;
}

void RoundedRectShape::setCornerPointCount(size_t count)
{
    _cornerPointCount = count;
    update();
}

size_t RoundedRectShape::getPointCount() const
{
    return _cornerPointCount * 4; // Four corners
}

sf::Vector2f RoundedRectShape::getPoint(size_t index) const
{
    if (index >= _cornerPointCount * 4)
        return sf::Vector2f(0, 0);

    float deltaAngle = 90.0f / (_cornerPointCount - 1);
    sf::Vector2f center;
    unsigned int centerIndex = index / _cornerPointCount;
    static const float pi = 3.141592654f;

    switch (centerIndex)
    {
    case 0:
        center.x = _size.x - _radius;
        center.y = _radius;
        break;
    case 1:
        center.x = _radius;
        center.y = _radius;
        break;
    case 2:
        center.x = _radius;
        center.y = _size.y - _radius;
        break;
    case 3:
        center.x = _size.x - _radius;
        center.y = _size.y - _radius;
        break;
    }

    return sf::Vector2f(_radius * cos(deltaAngle * (index - centerIndex) * pi / 180) + center.x,
                        -_radius * sin(deltaAngle * (index - centerIndex) * pi / 180) + center.y);
}

} // namespace Graphics
