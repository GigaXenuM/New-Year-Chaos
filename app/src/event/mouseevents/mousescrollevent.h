#pragma once

#include "event/event.h"

#include "SFML/System/Vector2.hpp"

class MouseScrollEvent : public Event
{
public:
    MouseScrollEvent(float delta, const sf::Vector2f &position);

    EventType type() const override
    {
        return EventType::MOUSE_SCROLL;
    }

    float delta() const;
    sf::Vector2f position() const;

private:
    float _delta;
    sf::Vector2f _position;
};
