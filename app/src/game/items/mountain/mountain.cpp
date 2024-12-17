#include "mountain.h"
#include "items/colliderfactory.h"

namespace Game
{

Mountain::Mountain(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::NonCollided>(world, { shape }) }
{
}

std::string Mountain::hintText() const
{
    return "I have to find a way around it";
}

bool Mountain::needDestroying() const
{
    return false;
}

sf::Vector2f Mountain::position() const
{
    return Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE);
}

} // namespace Game
