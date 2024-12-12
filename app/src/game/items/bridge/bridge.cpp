#include "bridge.h"

#include "items/colliderfactory.h"

#include <box2d/b2_polygon_shape.h>

namespace Game
{

Bridge::Bridge(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::Terrain>(world, { shape }) }
{
}

bool Bridge::needDestroying() const
{
    return false;
}

void Bridge::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
}

sf::Vector2f Bridge::position() const
{
    return Util::pointBy(boundingRect(), { Align::Left, Align::Bottom });
}

void Bridge::execute()
{
    collider()->SetType(b2BodyType::b2_dynamicBody);
}

} // namespace Game
