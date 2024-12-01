#include "physicalentity.h"

#include <box2d/b2_body.h>

namespace Game
{

PhysicalEntity::PhysicalEntity(b2Body *collider, const Context &context)
    : BaseType{ collider, context }
{
}

void PhysicalEntity::update(float deltatime)
{
    BaseType::update(deltatime);

    b2Vec2 velocity{ collider()->GetLinearVelocity() };

    velocity.x = 0.0f;
    if (isStateActive(State::Left))
        velocity.x -= context().velocity;
    if (isStateActive(State::Right))
        velocity.x += context().velocity;

    const bool onGround{ isStateActive(State::OnGround) };
    const bool needJumping{ isStateActive(State::Jump) && onGround };

    if (needJumping)
        velocity.y = 0.0f;

    if (velocity.x != 0.0f && velocity.y != 0.0f && onGround)
        velocity *= context().velocity / velocity.Length();
    collider()->SetLinearVelocity(velocity);

    if (needJumping)
    {
        b2Vec2 jumpImpulse(0.0f, -collider()->GetMass() * context().jumpImpulse);
        collider()->ApplyLinearImpulseToCenter(jumpImpulse, true);
    }
}

} // namespace Game
