#pragma once

#include "items/abstractphysicalitem.h"

namespace Game
{

enum class PhysicalBulletState
{
    Collide,
};

class PhysicalBullet : public AbstractPhysicalItem
{
public:
    using State = PhysicalBulletState;
    struct Context
    {
        float velocity{ 0.f };
        sf::Vector2f target;
    };

    explicit PhysicalBullet(b2Body *collider, const Context &context);

protected:
    void update(float deltatime) override;

private:
    const Context _context;
    const sf::Vector2f _startPosition;
};

} // namespace Game
