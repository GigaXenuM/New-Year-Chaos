#pragma once

#include "items/abstractphysicalitem.h"

namespace Game
{

class PhysicalBullet : public AbstractPhysicalItem
{
public:
    enum class State
    {
        Collide,
    };

    struct Context
    {
        float impulse{ 0.f };
        sf::Vector2f target;
    };

    explicit PhysicalBullet(b2Body *collider, const AbstractPhysicalItem *shooter,
                            const Context &context);

    ItemType type() const override
    {
        return ItemType::Bullet;
    }

    void updateState(State state, bool isActive);
    bool isStateActive(State state) const;

    void impulse();

    const AbstractPhysicalItem *shooter() const;

    virtual float power() const = 0;

private:
    const AbstractPhysicalItem *_shooter{ nullptr };
    const Context _context;
    const sf::Vector2f _startPosition;
    Util::EnumFlag<State> _state;
};

} // namespace Game
