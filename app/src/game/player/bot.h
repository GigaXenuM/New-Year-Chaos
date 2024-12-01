#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
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
    void updatePosition(float deltatime);
    void updateAnimation(float deltatime);
    void updateHealthBarPos(float deltatime);

    void initHealthValueRect();
    void updateHealthValueRectPos();

    const float _scale{ 0.40f };
    const float _scaleHealthBar{ 0.17f };
    const float _moveLimit{ 100.f };

    float _healthPoint{ 100.f };

    sf::Sprite _sprite;
    sf::Sprite _healthBar;
    sf::RectangleShape _health;

    sf::Vector2f _pos{};

    Animation _walkAnimation;
};
