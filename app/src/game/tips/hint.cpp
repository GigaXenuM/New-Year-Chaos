#include "hint.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "player/player.h"

#include <resources/resourcemanager.h>

namespace Game
{

Hint::Hint(const std::string &text)
{
    _text.setString(text);
}

void Hint::setPosition(const sf::Vector2f pos)
{
    _shape.setPosition(pos);
    _shape.setFillColor(sf::Color::White);
    _shape.setSize({ _text.getLocalBounds().width, 20 });

    _text.setFillColor(sf::Color(54, 255, 137));

    _text.setFont(ResourseManager::getInstance()->getFont(FontType::Arial));
    _text.setCharacterSize(18);
    _text.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = _text.getLocalBounds();
    _text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    _text.setPosition(pos);
}

void Hint::update(float deltatime)
{
    if (gPlayer)
        _shape.setPosition(
            { _shape.getPosition().x,
              _shape.getPosition().y - (gPlayer->getSprite().getGlobalBounds().height) });
}

void Hint::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_text, states);
    target.draw(_shape, states);
}
} // namespace Game
