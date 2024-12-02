#include "physicalbullet.h"

namespace Game
{

PhysicalBullet::PhysicalBullet(b2Body *collider, const Context &context)
    : AbstractPhysicalItem{ collider },
      _context{ context },
      _startPosition{ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE) }
{
}

void PhysicalBullet::update(float deltatime)
{
}

} // namespace Game
