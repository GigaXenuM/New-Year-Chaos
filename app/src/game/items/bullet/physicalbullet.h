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
        float velocity{ 0.f };
        sf::Vector2f target;
    };

    explicit PhysicalBullet(b2Body *collider, const Context &context);

    void updateState(State state, bool isActive);
    bool isStateActive(State state) const;

    void update(float deltatime) override;

private:
    const Context _context;
    const sf::Vector2f _startPosition;
    Util::EnumFlag<State> _state;
};

} // namespace Game
