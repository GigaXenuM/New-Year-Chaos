#include "teaicon.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "player/player.h"
#include "resources/resourcemanager.h"

namespace Game
{

TeaIcon::TeaIcon()
{
    _teaIcon.setTexture(ResourseManager::getInstance()->getTextures(TextureType::Tea)[0]);
    _teaIcon.setScale(0.48, 0.48);
    _text.setFont(ResourseManager::getInstance()->getFont(FontType::Arial));
    _text.setFillColor(sf::Color(54, 255, 137));
}

sf::FloatRect TeaIcon::globalRect() const
{
    return _teaIcon.getGlobalBounds();
}

void TeaIcon::setPosition(const sf::Vector2f pos)
{
    _teaIcon.setOrigin(Util::pointBy(_teaIcon.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _teaIcon.setPosition(pos);
    _text.setOrigin(Util::pointBy(_text.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _text.setPosition(pos + sf::Vector2f{ 0, 3 });
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
