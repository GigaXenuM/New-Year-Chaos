#pragma once

#include "animation/animation.h"
#include "items/entity/physicalentity.h"

#include <SFML/Graphics/Sprite.hpp>

#include <box2d/b2_body.h>

struct b2Body;

namespace Game
{

class Player : public PhysicalEntity
{
public:
    Player(b2Body *collider);

    sf::Vector2f getPosition() const;

    [[nodiscard]] bool isDead() const;
    [[nodiscard]] float getFreezPoints() const;
    [[nodiscard]] float getHealthPoints() const;

protected:
    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void updatePosition(float deltatime);
    void updateAnimation(float deltatime);
    void updateHealthPoint(float deltatime);

private:
    const float _scale{ 0.15f };
    const float _healthUpdateInterval{ 1.0f };

    float _freezPoint{ 100.f };
    float _healthPoint{ 100.f };

    float _healthUpdateTimer{ 0.0f };

    sf::Sprite _sprite;

    Animation _runAnimation;
    Animation _deadAnimation;
    Animation _walkAnimation;
    Animation _idleAnimation;
    Animation _jumpAnimation;
};

extern Player *gPlayer;

} // namespace Game
