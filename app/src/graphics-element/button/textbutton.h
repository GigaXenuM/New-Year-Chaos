#pragma once

#include "ibutton.h"
#include "item/ilayoutitem.h"

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

class EventHandler;

namespace Graphics
{

class TextButton : public ILayoutItem, public IButton
{
public:
    explicit TextButton(const sf::String &text, const sf::Font &font, const sf::Vector2f &size,
                        EventHandler *parent);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void setPosition(const sf::Vector2f &position) override;
    sf::Vector2f position() const override;
    void setOrigin(const Util::EnumFlag<Align> &origin) override;

    sf::FloatRect globalRect() const override;
    sf::FloatRect localRect() const override;

    void setTextColor(sf::Color color);
    void setBackgroundColor(sf::Color color);

    void onClick(OnClickCallback callback) override;

protected:
    void mousePressEvent(MousePressEvent *event) override;
    void mouseReleaseEvent(MouseReleaseEvent *event) override;

private:
    sf::Vector2f center() const;
    void setup();
    void updateGeometry();

    sf::String _text;

    sf::Font _font;
    sf::Text _textItem;
    sf::RectangleShape _shape;

    bool _pressed{ false };

    OnClickCallback _onClickCallback;
};
} // namespace Graphics
