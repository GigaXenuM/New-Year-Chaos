#pragma once

#include "animation/animation.h"
#include "item/phisicalitem.h"

#include <SFML/Graphics/Sprite.hpp>

class Bot : public Graphics::PhisicalItem
{
public:
    Bot(b2Body *collider, EventHandler *eventHandler);

protected:
    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void updateAnimation(float deltatime);
    void updatePosition(float deltatime);

    const float _scale{ 0.40f };
    const float _moveLimit{ 100.f };

    sf::Sprite _sprite;
    sf::Vector2f _pos{};
    Animation _walkAnimation;
};
