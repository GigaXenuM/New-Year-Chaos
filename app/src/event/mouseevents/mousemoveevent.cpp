#include "mousemoveevent.h"

MouseMoveEvent::MouseMoveEvent(PointF pos, PointF lastPos) : _pos{ pos }, _lastPos{ lastPos }
{
}

PointF MouseMoveEvent::position() const
{
    return _pos;
}

PointF MouseMoveEvent::lastPosition() const
{
    return _lastPos;
}
