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

    const float _scale{ 0.15f };

    sf::Sprite _sprite;

    Animation _walkAnimation;
};
