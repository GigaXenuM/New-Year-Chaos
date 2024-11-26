#pragma once

#include "event/event.h"

#include "event/mouseevents/mouse.h"

class MousePressEvent : public Event
{
public:
    MousePressEvent(Mouse::Button button, const sf::Vector2f &position);

    EventType type() const override
    {
        return EventType::MOUSE_PRESS;
    }

    Mouse::Button button() const;
    sf::Vector2f position() const;

private:
    Mouse::Button _button;
    sf::Vector2f _position;
};
