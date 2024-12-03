#include "snowball.h"

#include "items/colliderfactory.h"

namespace Game
{

SnowBall::SnowBall(b2World *world, sf::Shape *shape, const Context &context)
    : PhysicalBullet{ ColliderFactory::create<ItemType::SnowBall>(world, { shape }), context }
{
}

void SnowBall::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
}

} // namespace Game
