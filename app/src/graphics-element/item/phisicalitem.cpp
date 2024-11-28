#include "phisicalitem.h"

#include "box2d/b2_body.h"

namespace Graphics
{

PhisicalItem::PhisicalItem(b2Body *collider, const Context &context, EventHandler *parent)
    : Drawable{ parent }, _collider{ collider }, _context{ context }
{
}

void PhisicalItem::updateState(State state, bool isActive)
{
    isActive ? _state.set(state) : _state.unset(state);
}

bool PhisicalItem::isStateActive(State state) const
{
    return _state.test(state);
}

void PhisicalItem::update(float deltatime)
{
    b2Vec2 velocity{ _collider->GetLinearVelocity() };

    velocity.x = 0.0f;
    if (isStateActive(State::Left))
        velocity.x -= _context.velocity;
    if (isStateActive(State::Right))
        velocity.x += _context.velocity;

    const bool onGround{ isStateActive(State::OnGround) };
    const bool needJumping{ isStateActive(State::Jump) && onGround };
    if (needJumping)
        velocity.y = 0.0f;

    if (velocity.x != 0.0f && velocity.y != 0.0f && onGround)
        velocity *= _context.velocity / velocity.Length();
    _collider->SetLinearVelocity(velocity);

    if (isStateActive(State::Jump) && onGround)
    {
        b2Vec2 jumpImpulse(0.0f, -_collider->GetMass() * _context.jumpImpulse);
        _collider->ApplyLinearImpulseToCenter(jumpImpulse, true);
    }
}

const b2Body *PhisicalItem::collider()
{
    return _collider;
}
} // namespace Graphics
