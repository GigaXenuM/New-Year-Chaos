#pragma once

#include "animation/animation.h"
#include "item/phisicalitem.h"

#include <SFML/Graphics/Sprite.hpp>

#include <box2d/b2_body.h>

struct b2Body;

class Player : public Graphics::PhisicalItem
{
public:
    Player(b2Body *collider, EventHandler *eventHandler);

    sf::Vector2f getPosition() const;

protected:
    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void updateAnimation(float deltatime);
    void updatePosition(float deltatime);

private:
    const float _scale{ 0.15f };

    sf::Sprite _sprite;

    Animation _runAnimation;
    Animation _deadAnimation;
    Animation _walkAnimation;
    Animation _idleAnimation;
    Animation _jumpAnimation;
};
