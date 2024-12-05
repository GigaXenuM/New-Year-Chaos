#include "teaicon.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "player/player.h"
#include "resources/resourceManager.h"

namespace Game
{

TeaIcon::TeaIcon()
{
    _teaIcon.setTexture(ResourseManager::getInstance()->getTextures(TextureType::Tea)[0]);
    _teaIcon.setScale(0.03, 0.03);
    _text.setFont(ResourseManager::getInstance()->getFont(FontType::Arial));
    _text.setCharacterSize(30);
    _text.setFillColor(sf::Color::Red);
}

void TeaIcon::setPosition(const sf::Vector2f pos)
{
    _teaIcon.setPosition({ pos.x, pos.y - 3 });
    _text.setPosition({ pos.x + _teaIcon.getGlobalBounds().width, pos.y + 12 });
}

const sf::Sprite *TeaIcon::getSprite() const
{
    return &_teaIcon;
}

void TeaIcon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_teaIcon, states);
    target.draw(_text, states);
}

void TeaIcon::updateCountHealthItems()
{
    _text.setString(std::to_string(gPlayer->getHealthCount()));
}
} // namespace Game
