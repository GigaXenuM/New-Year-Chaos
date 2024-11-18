#pragma once

#include "event/event.h"

#include "event/mouseevents/mouse.h"

#include "geometry/point.h"

class MousePressEvent : public Event
{
public:
    MousePressEvent(Mouse::Button button, PointF position);

    EventType type() const override
    {
        return EventType::MOUSE_PRESS;
    }

    Mouse::Button button() const;
    PointF position() const;

private:
    Mouse::Button _button;
    PointF _position;
};
