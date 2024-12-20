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
    return "Мені варто знайти інший шлях";
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
