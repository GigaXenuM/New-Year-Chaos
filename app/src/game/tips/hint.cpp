#include "hint.h"

#include "resources/resourcemanager.h"
#include "util/geometryoperation.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

Hint::Hint(const std::string &text)
{
    _shape.setFillColor(sf::Color::White);
    _shape.setSize({ _text.getLocalBounds().width, 20 });

    _text.setString(text);
    _text.setFont(ResourseManager::getInstance()->getFont(FontType::Arial));
    _text.setCharacterSize(18);
    _text.setStyle(sf::Text::Bold);
    _text.setOrigin(Util::pointBy(_text.getLocalBounds(), Util::ALIGN_CENTER_STATE));
}

void Hint::setPosition(const sf::Vector2f pos)
{
    _shape.setPosition(pos);

    _text.setFillColor(sf::Color(54, 255, 137));

    _text.setPosition(pos);
}

void Hint::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_text, states);
    target.draw(_shape, states);
}
} // namespace Game
