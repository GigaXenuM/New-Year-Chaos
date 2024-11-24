#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "animation/animation.h"

#include <item/abstractitem.h>

class Player : public Graphics::AbstractItem
{
public:
    Player(EventHandler *eventHandler);

protected:
    RectF globalRect() const override;
    RectF localRect() const override;
    PointF center() const override;

    void setPos(PointF position) override;
    void setOrigin(Align origin) override;

    void keyPressEvent(KeyPressEvent *event) override;
    void keyReleaseEvent(KeyReleaseEvent *event) override;

    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void updateAnimation(const float deltatime);
    void updatePosition(const float deltatime);

private:
    const float _speed{ 200.f };
    const sf::Vector2f _scaleFactors{ 0.2f, 0.2f };
    const sf::Vector2f _rscaleFactors{ -0.2f, 0.2f };

    PointF _position{ 300, 300 };
    sf::Sprite _sprite;

    bool _runMode = { false };
    bool _slideMode = { false };

    bool _idleState = { true };
    bool _movingRight{ false };
    bool _movingLeft = { false };

private:
    Animation _runAnimation;
    Animation _deadAnimation;
    Animation _walkAnimation;
    Animation _idleAnimation;
    Animation _jumpAnimation;
};
