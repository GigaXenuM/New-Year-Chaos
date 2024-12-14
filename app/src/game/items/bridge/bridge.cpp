#include "bridge.h"

#include "items/bridge/bridgedrawablejoint.h"
#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include <box2d/b2_polygon_shape.h>

namespace Game
{

Bridge::Bridge(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::Terrain>(world, { shape }) },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Bridge).front() },
      _postDrawElement{ new BridgeDrawableJoint(this) },
      _actionPos{ Util::pointBy(_postDrawElement->_chainSprite.getGlobalBounds(),
                                { Align::Bottom, Align::Left }) }

{
    constexpr float scale{ 0.5f };
    _sprite.setScale(scale, scale);
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
}

bool Bridge::needDestroying() const
{
    return false;
}

void Bridge::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

void Bridge::update(float deltatime)
{
    constexpr float RAD_TO_DEG{ 180.f / b2_pi };
    const float angleInDegrees{ collider()->GetAngle() * RAD_TO_DEG + _rotationOffset };

    _sprite.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));
    _sprite.setRotation(angleInDegrees);
}

Graphics::Drawable *Bridge::postDrawElement() const
{
    return _postDrawElement;
}

} // namespace Game
