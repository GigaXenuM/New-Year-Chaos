#pragma once

#include "event/event.h"

#include "event/keyevents/keyboard.h"

class KeyReleaseEvent : public Event
{
public:
    explicit KeyReleaseEvent(Keyboard::Key key, bool shift, bool alt, bool control);
    virtual ~KeyReleaseEvent() = default;

    EventType type() const override
    {
        return EventType::KEY_RELEASE;
    }

    Keyboard::Key key() const;
    Keyboard::Mode mode() const;

private:
    Keyboard::Key _key;
    Keyboard::Mode _mode{ Keyboard::Mode::None };
};
