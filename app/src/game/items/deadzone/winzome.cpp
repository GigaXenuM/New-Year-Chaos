#include "winzome.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "items/colliderfactory.h"
#include <resources/resourcemanager.h>

namespace Game
{

WinZone::WinZone(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::WinZone>(world, { shape }) },
      _sprite(ResourseManager::getInstance()->getTextures(TextureType::Win_gift_zone)[0])
{
}

std::string WinZone::hintText() const
{
    return "Хах... це було легше чим я собі думав!\n Давай подаруємо цьому світу свято\n натисни "
           "[У] "
           "для завершення гри.";
}

bool WinZone::needDestroying() const
{
    return false;
}

void WinZone::update(float deltatime)
{
    const sf::Vector2f winZonePos{ Util::pointBy(boundingRect(), { Align::Bottom }) };
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), { Align::Bottom }));
    _sprite.setPosition(winZonePos);
}

void WinZone::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

} // namespace Game
