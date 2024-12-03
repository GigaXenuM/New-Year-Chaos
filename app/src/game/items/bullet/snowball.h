#pragma once

#include "physicalbullet.h"

namespace Game
{

class SnowBall : public PhysicalBullet
{
public:
    explicit SnowBall(b2World *world, sf::Shape *shape, const Context &context);

    ItemType type() const override
    {
        return ItemType::SnowBall;
    }
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

} // namespace Game
