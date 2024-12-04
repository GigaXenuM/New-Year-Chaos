#pragma once

#include "animation/animation.h"
#include "physicalbullet.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Game
{

class SnowBall : public PhysicalBullet
{
public:
    explicit SnowBall(b2World *world, sf::Shape *shape, const AbstractPhysicalItem *shooter,
                      const Context &context);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void update(float deltatime) override;

    float power() const override;

private:
    void updatePosition();
    void updateAnimation(float deltatime);

    Animation _flyAnimation;
    const float _scale{ 0.10f };
    sf::Sprite _sprite;
};

} // namespace Game
