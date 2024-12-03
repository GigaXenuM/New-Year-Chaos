#pragma once

#include "items/abstractphysicalitem.h"

#include "util/enumflag.h"

#include <memory>

struct b2Body;

namespace Game
{

class PhysicalBullet;

class PhysicalEntity : public AbstractPhysicalItem
{
public:
    enum class State
    {
        OnGround,
        Left,
        Right,
        Jump,
    };

    struct Context
    {
        float velocity{ 0.f };
        float jumpImpulse{ 0.f };
    };

    explicit PhysicalEntity(b2Body *collider, const Context &context);
    ~PhysicalEntity();

    void updateState(State state, bool isActive);
    bool isStateActive(State state) const;

    void shoot(const sf::Vector2f &target);

protected:
    void update(float deltatime) override;

private:
    const Context _context;
    Util::EnumFlag<State> _state;
    std::vector<std::unique_ptr<PhysicalBullet>> _bullets;
};

} // namespace Game
