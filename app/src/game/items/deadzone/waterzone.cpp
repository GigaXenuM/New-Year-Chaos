#include "waterzone.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "items/colliderfactory.h"

#include <resources/resourcemanager.h>

namespace Game
{

WaterZone::WaterZone(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::WaterZone>(world, { shape }) },
      _divingAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Dead_water_zone) },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Dead_water_zone)[0] }
{
}

bool WaterZone::needDestroying() const
{
    return false;
}

void WaterZone::playAnimation()
{
    _isNeedPlayAnim = true;
}

void WaterZone::update(float deltatime)
{
    if (_isNeedPlayAnim)
    {
        _divingAnimation.start(deltatime, _sprite, false);
        _isNeedPlayAnim = !_divingAnimation.isFinished();
    }

    const sf::Vector2f deadZonepos{ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE) };
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setPosition(deadZonepos);
}

void WaterZone::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (_isNeedPlayAnim)
        target.draw(_sprite, states);
}

} // namespace Game
