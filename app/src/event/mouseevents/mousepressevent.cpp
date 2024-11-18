#include "mousepressevent.h"

MousePressEvent::MousePressEvent(Mouse::Button button, PointF position)
    : _button{ button }, _position{ position }
{
}

Mouse::Button MousePressEvent::button() const
{
    return _button;
}

PointF MousePressEvent::position() const
{
    return _position;
}
