#include "physicalbullet.h"

namespace Game
{

PhysicalBullet::PhysicalBullet(b2Body *collider, const AbstractPhysicalItem *shooter,
                               const Context &context)
    : AbstractPhysicalItem{ collider },
      _shooter{ shooter },
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

void PhysicalBullet::impulse()
{
    sf::Vector2f direction = _context.target - _startPosition;

    b2Vec2 impulse{ direction.x, direction.y };
    impulse.Normalize();
    impulse *= _context.impulse;

    collider()->ApplyLinearImpulseToCenter(impulse, true);
}

const AbstractPhysicalItem *PhysicalBullet::shooter() const
{
    return _shooter;
}

} // namespace Game
