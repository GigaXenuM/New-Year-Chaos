#pragma once

#include "SFML/Graphics/Sprite.hpp"

#include "physicalbullet.h"

namespace Game
{

class AxeBullet : public PhysicalBullet
{
public:
    explicit AxeBullet(b2World *world, sf::Shape *shape, const AbstractPhysicalItem *shooter,
                       const Context &context);

    void update(float deltatime) override;
    float power() const override;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void updatePosition(float deltatime);

private:
    const float _scale{ 0.10f };
    sf::Sprite _sprite;
};
} // namespace Game