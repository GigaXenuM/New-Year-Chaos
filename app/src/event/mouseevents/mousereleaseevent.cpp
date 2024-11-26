#include "mousereleaseevent.h"

MouseReleaseEvent::MouseReleaseEvent(Mouse::Button button, const sf::Vector2f &position)
    : _button{ button }, _position{ position }
{
}

Mouse::Button MouseReleaseEvent::button() const
{
    return _button;
}

sf::Vector2f MouseReleaseEvent::position() const
{
    return _position;
}
