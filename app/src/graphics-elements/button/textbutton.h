#pragma once

#include "ibutton.h"
#include "scene/items/abstractitem.h"

#include "geometry/point.h"
#include "geometry/rect.h"
#include "geometry/size.h"

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

#include <string>

class EventHandler;

namespace Scene
{

class TextButton : public AbstractItem, public IButton
{
public:
    explicit TextButton(const std::string &text, const sf::Font &font, SizeF size,
                        EventHandler *parent);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void setPos(PointF position) override;
    void setOrigin(Align origin) override;

    RectF globalRect() const override;
    RectF localRect() const override;
    PointF center() const override;

    void setTextColor(sf::Color color);
    void setBackgroundColor(sf::Color color);

    void onClick(OnClickCallback callback) override;

protected:
    void mousePressEvent(MousePressEvent *event) override;
    void mouseReleaseEvent(MouseReleaseEvent *event) override;

private:
    void setup();
    void updateGeometry();

    std::string _text;

    sf::Font _font;
    sf::Text _textItem;
    sf::RectangleShape _shape;

    bool _pressed{ false };

    OnClickCallback _onClickCallback;
};
} // namespace Scene
