#pragma once

#include "layout.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class EventHandler;

class VerticalLayout : public Layout
{
public:
    explicit VerticalLayout(const sf::FloatRect &rect = {});

protected:
    void updateGeometry() override;

private:
    sf::Vector2f contentSize() const;

    void alignCenter();
    void alignLeft();
    void alignRight();
    void alignTop();
    void alignBottom();

    sf::Vector2f _nextItemPos{ 0.f, 0.f };
};
