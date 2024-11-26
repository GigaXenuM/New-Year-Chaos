#pragma once

#include "event/event.h"

#include "SFML/System/Vector2.hpp"

class MouseMoveEvent : public Event
{
public:
    explicit MouseMoveEvent(const sf::Vector2f &pos, const sf::Vector2f &lastPos);

    EventType type() const override
    {
        return EventType::MOUSE_MOVE;
    }

    sf::Vector2f position() const;
    sf::Vector2f lastPosition() const;

private:
    sf::Vector2f _pos;
    sf::Vector2f _lastPos;
};
