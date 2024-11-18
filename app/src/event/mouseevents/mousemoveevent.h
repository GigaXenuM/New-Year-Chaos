#pragma once

#include "event/event.h"

#include "geometry/point.h"

class MouseMoveEvent : public Event
{
public:
    explicit MouseMoveEvent(PointF pos, PointF lastPos);

    EventType type() const override
    {
        return EventType::MOUSE_MOVE;
    }

    PointF position() const;
    PointF lastPosition() const;

private:
    PointF _pos;
    PointF _lastPos;
};
