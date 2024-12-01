#pragma once

#include "items/abstractphysicalitem.h"

namespace Game
{

enum class PhysicalEntityState
{
    OnGround,
    Left,
    Right,
    Jump,
};

class PhysicalEntity : public AbstractPhysicalItem<PhysicalEntityState>
{
public:
    using State = PhysicalEntityState;

    PhysicalEntity(b2Body *collider, const Context &context);

protected:
    void update(float deltatime) override;
};

} // namespace Game
