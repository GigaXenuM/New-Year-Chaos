#pragma once

#include "items/abstractphysicalitem.h"

#include "util/enumflag.h"

#include <memory>

struct b2Body;

namespace Game
{
class IWeapon;

class PhysicalEntity : public AbstractPhysicalItem
{
public:
    enum class State
    {
        OnGround,
        PrepareGroundDetach,
        Left,
        Right,
        Run,
        Jump,
        Dead,
        RemoveMe,
    };

    struct Context
    {
        float velocity{ 0.f };
        float jumpImpulse{ 0.f };
    };

    explicit PhysicalEntity(b2Body *collider, const Context &context);
    ~PhysicalEntity();

    ItemType type() const override
    {
        return ItemType::Entity;
    }
    void updatePhysics() override;

    void updateState(State state, bool isActive);
    bool isStateActive(State state) const;

    void shoot(const sf::Vector2f &target);

    virtual void damage(float power) = 0;

    IWeapon *const weapon() const;

protected:
    void update(float deltatime) override;

private:
    const Context _context;
    std::unique_ptr<IWeapon> _weapon;

    Util::EnumFlag<State> _state;
};

} // namespace Game
