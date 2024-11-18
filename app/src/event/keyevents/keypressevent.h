#pragma once

#include "event/event.h"

#include "event/keyevents/keyboard.h"

class KeyPressEvent : public Event
{
public:
    explicit KeyPressEvent(Keyboard::Key key, bool shift, bool alt, bool control);
    virtual ~KeyPressEvent() = default;

    EventType type() const override
    {
        return EventType::KEY_PRESS;
    }

    Keyboard::Key key() const;
    Keyboard::Mode mode() const;

private:
    Keyboard::Key _key;
    Keyboard::Mode _mode{ Keyboard::Mode::None };
};
