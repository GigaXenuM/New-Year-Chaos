#include "hint.h"

#include "resources/resourcemanager.h"
#include "util/geometryoperation.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

namespace
{
constexpr float DRAW_CHARACTER_TIME{ 0.02f };
constexpr unsigned CHARACTER_SIZE{ 18 };
constexpr unsigned CORNER_RADIUS{ 20 };
constexpr unsigned SHAPE_MARGIN{ CHARACTER_SIZE * 2 };
const sf::Color TEXT_COLOR{ 54, 255, 137 };
const sf::Color BACKGROUND_COLOR{ 66, 59, 65 };
} // namespace

Hint::Hint(const std::string &text) : _shape{ {}, CORNER_RADIUS }
{
    _shape.setFillColor(BACKGROUND_COLOR);

    _textItem.setFont(ResourseManager::getInstance()->getFont(FontType::Arial));
    _textItem.setCharacterSize(CHARACTER_SIZE);
    _textItem.setStyle(sf::Text::Bold);
    _textItem.setFillColor(TEXT_COLOR);

    setText(text);
}

bool Hint::empty() const
{
    return _text.empty();
}

void Hint::setText(const std::string &text)
{
    if (_text == text)
        return;

    _text = text;
    _textItem.setString("");
    _characterIndex = 0;
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
    target.draw(_textItem, states);
}

void Hint::update(float deltatime)
{
    if (_text.empty() || _text == _textItem.getString() || _characterIndex >= _text.size())
        return;

    _drawTextDuration += deltatime;
    if (_drawTextDuration < DRAW_CHARACTER_TIME)
        return;

    _drawTextDuration = 0.f;
    const std::string text{ _text.substr(0, _characterIndex++ + 1) };
    _textItem.setString(text);

    updateGeometry();
}

void Hint::updateGeometry()
{
    const sf::FloatRect textLocalRect{ _textItem.getLocalBounds() };
    const sf::Vector2 textSize{ textLocalRect.getSize() };

    const sf::FloatRect shapeLocalRect{ _shape.getLocalBounds() };
    const sf::FloatRect shapeGlobalRect{ _shape.getGlobalBounds() };
    const sf::Vector2 shapeNewSize{ textSize.x + SHAPE_MARGIN, textSize.y + SHAPE_MARGIN };

    _shape.setSize(shapeNewSize);
    _shape.setOrigin(Util::pointBy(shapeLocalRect, { Align::Bottom }));

    _textItem.setOrigin(Util::pointBy(textLocalRect, Util::ALIGN_CENTER_STATE));
    _textItem.setPosition(Util::pointBy(shapeGlobalRect, Util::ALIGN_CENTER_STATE));
}
} // namespace Game
