#include "physicalbullet.h"

namespace Game
{

PhysicalBullet::PhysicalBullet(b2Body *collider, const Context &context)
    : AbstractPhysicalItem{ collider },
      _context{ context },
      _startPosition{ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE) }
{
}

void PhysicalBullet::updateState(State state, bool isActive)
{
    isActive ? _state.set(state) : _state.unset(state);
}

bool PhysicalBullet::isStateActive(State state) const
{
    return _state.test(state);
}

void PhysicalBullet::update(float deltatime)
{
    const b2Vec2 startPos{ _startPosition.x, _startPosition.y };
    const b2Vec2 targetPos{ _context.target.x, _context.target.y };
    b2Vec2 direction = targetPos - startPos;

    direction.Normalize();
    direction *= 10.f;

    collider()->ApplyLinearImpulseToCenter(direction, true);
}

} // namespace Game
