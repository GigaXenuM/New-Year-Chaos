#include "bridgedrawablejoint.h"

#include "items/bridge/bridge.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Game
{

BridgeDrawableJoint::BridgeDrawableJoint(const Bridge *bridge)
    : _bridge{ bridge },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Bridge).back() },
      _chainSprite{ ResourseManager::getInstance()->getTextures(TextureType::Chain).front() }
{

    _chainSprite.scale(0.6, 0.6);
    _chainSprite.setOrigin(Util::pointBy(_chainSprite.getLocalBounds(), { Align::Top }));
    _chainSprite.setRotation(30.f);
    _chainSprite.setPosition(Util::pointBy(_bridge->boundingRect(), { Align::Left }));
}

void BridgeDrawableJoint::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (_bridge->type() == ItemType::TerrainObstacle)
        target.draw(_chainSprite, states);

    target.draw(_sprite, states);
}

void BridgeDrawableJoint::update(float)
{
    _sprite.setScale(_bridge->_sprite.getScale());
    _sprite.setOrigin(_bridge->_sprite.getOrigin());
    _sprite.setRotation(_bridge->_sprite.getRotation());
    _sprite.setPosition(_bridge->_sprite.getPosition());
}

} // namespace Game
