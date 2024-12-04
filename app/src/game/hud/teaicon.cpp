#include "teaicon.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "resources/resourceManager.h"

namespace Game
{

TeaIcon::TeaIcon()
{
    _teaIcon.setTexture(ResourseManager::getInstance()->getTextures(TextureType::Tea)[0]);
    _teaIcon.setScale(0.03, 0.03);
}

void TeaIcon::setPosition(const sf::Vector2f pos)
{
    _teaIcon.setPosition({ pos.x, pos.y });
}

const sf::Sprite *TeaIcon::getSprite() const
{
    return &_teaIcon;
}

void TeaIcon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_teaIcon, states);
}

} // namespace Game
