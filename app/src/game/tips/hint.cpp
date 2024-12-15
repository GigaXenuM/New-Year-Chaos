#include "hint.h"

#include "resources/resourcemanager.h"
#include "util/geometryoperation.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

namespace
{
constexpr unsigned CHARACTER_SIZE{ 18 };
constexpr unsigned CORNER_RADIUS{ 20 };
constexpr unsigned SHAPE_MARGIN{ CHARACTER_SIZE * 2 };
const sf::Color TEXT_COLOR{ 54, 255, 137 };
const sf::Color BACKGROUND_COLOR{ 66, 59, 65 };
} // namespace

Hint::Hint(const std::string &text) : _shape{ {}, CORNER_RADIUS }
{
    _shape.setFillColor(BACKGROUND_COLOR);

    _text.setFont(ResourseManager::getInstance()->getFont(FontType::Arial));
    _text.setCharacterSize(CHARACTER_SIZE);
    _text.setStyle(sf::Text::Bold);
    _text.setFillColor(TEXT_COLOR);

    setText(text);
}

bool Hint::empty() const
{
    return _text.getString().isEmpty();
}

void Hint::setText(const std::string &text)
{
    _text.setString(text);
    updateGeometry();
}

void Hint::setPosition(const sf::Vector2f pos)
{
    if (empty())
        return;

    _shape.setPosition(pos);
    updateGeometry();
}

void Hint::reset()
{
    setText({});
}

void Hint::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (empty())
        return;

    target.draw(_shape, states);
    target.draw(_text, states);
}

void Hint::updateGeometry()
{
    const sf::FloatRect textLocalRect{ _text.getLocalBounds() };
    const sf::Vector2 textSize{ textLocalRect.getSize() };

    const sf::FloatRect shapeLocalRect{ _shape.getLocalBounds() };
    const sf::FloatRect shapeGlobalRect{ _shape.getGlobalBounds() };
    const sf::Vector2 shapeNewSize{ textSize.x + SHAPE_MARGIN, textSize.y + SHAPE_MARGIN };

    _shape.setSize(shapeNewSize);
    _shape.setOrigin(Util::pointBy(shapeLocalRect, { Align::Bottom }));

    _text.setOrigin(Util::pointBy(textLocalRect, Util::ALIGN_CENTER_STATE));
    _text.setPosition(Util::pointBy(shapeGlobalRect, Util::ALIGN_CENTER_STATE));
}
} // namespace Game
