#include "keyloot.h"

#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

KeyLoot::KeyLoot(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::Loot>(world, { shape }) },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Key).front() }
{
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setScale(0.2, 0.2);
}

std::string KeyLoot::hintText() const
{
    return "Press E to pickup";
}

bool KeyLoot::needDestroying() const
{
    return _needDestroy;
}

void KeyLoot::prepareDestroy()
{
    _needDestroy = true;
}

void KeyLoot::update(float deltatime)
{
    _sprite.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));
}

void KeyLoot::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

sf::Vector2f KeyLoot::position() const
{
    return Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE);
}

void KeyLoot::execute()
{
    prepareDestroy();
}

} // namespace Game
