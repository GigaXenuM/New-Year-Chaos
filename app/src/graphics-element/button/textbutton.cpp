#include "textbutton.h"

#include "event/mouseevents/mousepressevent.h"
#include "event/mouseevents/mousereleaseevent.h"

#include "geometry/alignment.h"
#include "util/enumflag.h"
#include "util/geometryoperation.h"

#include "SFML/Graphics/RenderTarget.hpp"

namespace
{
const sf::Color DEFAULT_BACKGROUND_COLOR{ 38, 35, 36, 255 };
const sf::Color ACTIVE_BACKGROUND_COLOR{ 140, 34, 179, 255 };
} // namespace

namespace Graphics
{
TextButton::TextButton(const sf::String &text, const sf::Font &font, const sf::Vector2f &size,
                       EventHandler *parent)
    : ILayoutItem{ parent },
      _text{ text },
      _font{ font },
      _textItem{ _text, _font },
      _shape{ size },
      _onClickCallback{ []() {} }
{
    setup();
    updateGeometry();
}

void TextButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_shape, states);
    target.draw(_textItem, states);
}

void TextButton::setPosition(const sf::Vector2f &position)
{
    _shape.setPosition(position);
    updateGeometry();
}

sf::Vector2f TextButton::position() const
{
    return _shape.getPosition();
}

void TextButton::setOrigin(const Util::EnumFlag<Align> &origin)
{
    const auto rect{ localRect() };
    _shape.setOrigin(Util::pointBy(rect, origin));
}

sf::FloatRect TextButton::globalRect() const
{
    return _shape.getGlobalBounds();
}

sf::FloatRect TextButton::localRect() const
{
    return _shape.getLocalBounds();
}

sf::Vector2f TextButton::center() const
{
    const sf::Transform &transform = _shape.getTransform();
    return transform.transformPoint(_shape.getSize().x / 2, _shape.getSize().y / 2);
}

void TextButton::setTextColor(sf::Color color)
{
    _textItem.setFillColor(color);
}

void TextButton::setBackgroundColor(sf::Color color)
{
    _shape.setFillColor(color);
}

void TextButton::onClick(OnClickCallback callback)
{
    _onClickCallback = callback;
}

void TextButton::mousePressEvent(MousePressEvent *event)
{
    if (event->button() != Mouse::Button::Left)
        return;

    sf::FloatRect rect{ globalRect() };
    bool mouseHovered{ event->position().x > rect.getPosition().x
                       && event->position().x < rect.getPosition().x + rect.width
                       && event->position().y > rect.getPosition().y
                       && event->position().y < rect.getPosition().y + rect.height };

    if (mouseHovered)
    {
        _shape.setFillColor(ACTIVE_BACKGROUND_COLOR);
        _pressed = true;
    }
}

void TextButton::mouseReleaseEvent(MouseReleaseEvent *event)
{
    if (event->button() != Mouse::Button::Left)
        return;

    if (_pressed)
    {
        _shape.setFillColor(DEFAULT_BACKGROUND_COLOR);
        _onClickCallback();
        _pressed = false;
    }
}

void TextButton::setup()
{
    _shape.setFillColor(DEFAULT_BACKGROUND_COLOR);

    sf::FloatRect textBound{ _textItem.getLocalBounds() };
    sf::FloatRect shapeBound{ _shape.getLocalBounds() };

    if (textBound.width > shapeBound.width)
        _shape.setSize({ textBound.width, shapeBound.height });
    if (textBound.height > shapeBound.height)
        _shape.setSize({ shapeBound.width, textBound.height });

    sf::Vector2f textCenter{ textBound.left + textBound.width / 2.f,
                             textBound.top + textBound.height / 2.f };
    _textItem.setOrigin(textCenter);
}

void TextButton::updateGeometry()
{
    _textItem.setPosition(center());
}
} // namespace Graphics
