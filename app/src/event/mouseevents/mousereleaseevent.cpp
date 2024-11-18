#include "mousereleaseevent.h"

MouseReleaseEvent::MouseReleaseEvent(Mouse::Button button, PointF position)
    : _button{ button }, _position{ position }
{
}

Mouse::Button MouseReleaseEvent::button() const
{
    return _button;
}

PointF MouseReleaseEvent::position() const
{
    return _position;
}
