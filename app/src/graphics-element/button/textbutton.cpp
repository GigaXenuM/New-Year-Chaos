#include "textbutton.h"

#include "event/mouseevents/mousepressevent.h"
#include "event/mouseevents/mousereleaseevent.h"

#include "geometry/alignment.h"
#include "geometry/utils.h"

#include "SFML/Graphics/RenderTarget.hpp"

namespace
{
const sf::Color DEFAULT_BACKGROUND_COLOR{ 38, 35, 36, 255 };
const sf::Color ACTIVE_BACKGROUND_COLOR{ 140, 34, 179, 255 };
} // namespace

namespace Graphics
{
TextButton::TextButton(const sf::String &text, const sf::Font &font, SizeF size,
                       EventHandler *parent)
    : AbstractItem{ parent },
      _text{ text },
      _font{ font },
      _textItem{ _text, _font },
      _shape{ Geometry::toSfmlSize(size) },
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

void TextButton::setPos(PointF position)
{
    _shape.setPosition(Geometry::toSfmlPoint(position));
    updateGeometry();
}

void TextButton::setOrigin(Align origin)
{
    _shape.setOrigin(Geometry::toSfmlPoint(localRect().pointBy(origin)));
}

RectF TextButton::globalRect() const
{
    return Geometry::toRect(_shape.getGlobalBounds());
}

RectF TextButton::localRect() const
{
    return Geometry::toRect(_shape.getLocalBounds());
}

PointF TextButton::center() const
{
    const sf::Transform &transform = _shape.getTransform();
    return Geometry::toPoint(
        transform.transformPoint(_shape.getSize().x / 2, _shape.getSize().y / 2));
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

    RectF rect{ globalRect() };
    bool mouseHovered{ event->position().x() > rect.pos.x()
                       && event->position().x() < rect.pos.x() + rect.width()
                       && event->position().y() > rect.pos.y()
                       && event->position().y() < rect.pos.y() + rect.height() };

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
    _textItem.setPosition(Geometry::toSfmlPoint(center()));
}
} // namespace Graphics
