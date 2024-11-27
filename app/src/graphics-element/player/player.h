#pragma once

#include "animation/animation.h"
#include "item/phisicalitem.h"

#include <SFML/Graphics/Sprite.hpp>

class Player : public Graphics::PhisicalItem
{
public:
    Player(sf::RectangleShape *collider, EventHandler *eventHandler);

    sf::Vector2f getPosition() const;

protected:
    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void updateAnimation(float deltatime);
    void updatePosition(float deltatime);

private:
    const float _speed{ 200.f };
    const float _scale{ 0.15f };

    sf::Sprite _sprite;

    Animation _runAnimation;
    Animation _deadAnimation;
    Animation _walkAnimation;
    Animation _idleAnimation;
    Animation _jumpAnimation;
};
