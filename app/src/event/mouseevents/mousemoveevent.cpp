#include "mousemoveevent.h"

MouseMoveEvent::MouseMoveEvent(const sf::Vector2f &pos, const sf::Vector2f &lastPos)
    : _pos{ pos }, _lastPos{ lastPos }
{
}

sf::Vector2f MouseMoveEvent::position() const
{
    return _pos;
}

sf::Vector2f MouseMoveEvent::lastPosition() const
{
    return _lastPos;
}
