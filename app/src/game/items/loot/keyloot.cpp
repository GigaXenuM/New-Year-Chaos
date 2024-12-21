#include "keyloot.h"

#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

TextureType textureTypeBy(ActionVariant variant)
{
    switch (variant)
    {
    case ActionVariant::PickUpKeyBridge:
        return TextureType::KeyBridge;
    case ActionVariant::PickUpTea:
        return TextureType::Tea;
    case ActionVariant::PickUpKeyDoor:
        return TextureType::KeyDoor;
    default:
        assert(false && "Is not a loot");
    }

    return {};
}

std::string hintTextBy(ActionVariant variant)
{
    switch (variant)
    {
    case ActionVariant::PickUpKeyBridge:
        [[fallthrough]];
    case ActionVariant::PickUpKeyDoor:
        return "[У] - взяти ключ";
    case ActionVariant::PickUpTea:
        return "[У] - взяти чай";
    default:
        assert(false && "Is not a loot");
    }

    return {};
}

KeyLoot::KeyLoot(b2World *world, sf::Shape *shape, ActionVariant variant)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::Loot>(world, { shape }) },
      _sprite{ ResourseManager::getInstance()->getTextures(textureTypeBy(variant)).front() },
      _variant{ variant }
{
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setScale(0.2, 0.2);
}

ActionVariant KeyLoot::actionVariant() const
{
    return _variant;
}

std::string KeyLoot::hintText() const
{
    return hintTextBy(_variant);
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
