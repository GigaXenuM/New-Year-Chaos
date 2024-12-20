#include "healthloot.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"
namespace Game
{

HealthLoot::HealthLoot(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::Loot>(world, { shape }) },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Aid_Kit_loot).front() }
{
    _sprite.setScale({ 0.1, 0.1 });
}

std::string HealthLoot::hintText() const
{
    return "Press E to pickup";
}

bool HealthLoot::needDestroying() const
{
    return _needDestroy;
}

void HealthLoot::prepareDestroy()
{
    _needDestroy = true;
}

void HealthLoot::update(float deltatime)
{
    _sprite.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));
}

void HealthLoot::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

sf::Vector2f HealthLoot::position() const
{
    return Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE);
}

void HealthLoot::execute()
{
    prepareDestroy();
}
} // namespace Game