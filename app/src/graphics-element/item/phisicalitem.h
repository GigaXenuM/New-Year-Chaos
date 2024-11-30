#pragma once

#include "item/drawable.h"

#include "util/enumflag.h"

struct b2Body;

namespace Graphics
{

class PhisicalItem : public Drawable
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

    PhisicalItem(b2Body *collider, const Context &context);

    void updateState(State state, bool isActive);
    bool isStateActive(State state) const;

    sf::FloatRect boundingRect() const;

protected:
    void update(float deltatime) override;

    const b2Body *collider();

private:
    b2Body *_collider{ nullptr };
    const Context _context;
    Util::EnumFlag<State> _state;
};

} // namespace Graphics
