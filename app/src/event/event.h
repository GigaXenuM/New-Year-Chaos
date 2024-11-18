#pragma once

class Event
{
public:
    enum class EventType
    {
        KEY_PRESS,
        KEY_RELEASE,
        MOUSE_MOVE,
        MOUSE_PRESS,
        MOUSE_RELEASE,
        MOUSE_SCROLL
    };

    virtual ~Event() = default;

    virtual EventType type() const = 0;
};
