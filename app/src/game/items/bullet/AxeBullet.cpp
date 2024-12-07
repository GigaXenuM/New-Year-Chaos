#include "AxeBullet.h"

#include "items/colliderfactory.h"

namespace Game
{

AxeBullet::AxeBullet(b2World *world, sf::Shape *shape, const AbstractPhysicalItem *shooter,
                     const Context &context)
    : PhysicalBullet{ ColliderFactory::create<ItemType::Bullet>(world, { shape }), shooter,
                      context }
{
}

void AxeBullet::update(float deltatime)
{
    PhysicalBullet::update(deltatime);
}

float AxeBullet::power() const
{
}
} // namespace Game