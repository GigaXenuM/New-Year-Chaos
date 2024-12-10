#include "waterzone.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "items/colliderfactory.h"
#include "player/player.h"

#include <resources/resourcemanager.h>

namespace Game
{

DeadWaterZone::DeadWaterZone(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::WaterZone>(world, { shape }) },
      _divingAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Dead_water_zone) },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Dead_water_zone)[0] }
{
}

void DeadWaterZone::playAnimation()
{
    _isNeedPlayAnim = true;
}

void DeadWaterZone::update(float deltatime)
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

void DeadWaterZone::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (_isNeedPlayAnim)
        target.draw(_sprite, states);
}

} // namespace Game
