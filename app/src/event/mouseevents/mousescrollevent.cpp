#include "mousescrollevent.h"

MouseScrollEvent::MouseScrollEvent(float delta, const sf::Vector2f &position)
    : _delta{ delta }, _position{ position }
{
}

float MouseScrollEvent::delta() const
{
    return _delta;
}

sf::Vector2f MouseScrollEvent::position() const
{
    return _position;
}
