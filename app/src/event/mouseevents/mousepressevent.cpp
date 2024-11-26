#include "mousepressevent.h"

MousePressEvent::MousePressEvent(Mouse::Button button, const sf::Vector2f &position)
    : _button{ button }, _position{ position }
{
}

Mouse::Button MousePressEvent::button() const
{
    return _button;
}

sf::Vector2f MousePressEvent::position() const
{
    return _position;
}
