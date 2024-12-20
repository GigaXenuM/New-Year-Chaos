#include "teaicon.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "player/player.h"
#include "resources/resourcemanager.h"

namespace Game
{

TeaIcon::TeaIcon()
{
    _teaIcon.setScale(0.48, 0.48);
    _text.setFont(ResourseManager::getInstance()->getFont(FontType::DejaVuSansBold));
    _text.setFillColor(sf::Color(54, 255, 137));
}

sf::FloatRect TeaIcon::globalRect() const
{
    return _teaIcon.getGlobalBounds();
}

void TeaIcon::setTexture(sf::Texture &texture)
{
    _teaIcon.setTexture(texture);
}

void TeaIcon::setPosition(const sf::Vector2f pos)
{
    _teaIcon.setOrigin(Util::pointBy(_teaIcon.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _teaIcon.setPosition({ pos.x, pos.y - _teaIcon.getGlobalBounds().height / 2 });
    _text.setOrigin(Util::pointBy(_text.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _text.setPosition({ pos.x, pos.y + _text.getGlobalBounds().height * 1.2f });
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

void TeaIcon::updateHealCount(size_t healCount)
{
    _text.setString(std::to_string(healCount));
}

void TeaIcon::setScaleFactor(float x, float y)
{
    _teaIcon.setScale(x, y);
}
} // namespace Game
