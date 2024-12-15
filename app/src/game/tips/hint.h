#pragma once

#include "item/abstractitem.h"

#include "shape/roundedrectshape.h"

#include <SFML/Graphics/Text.hpp>

namespace Game
{
class Hint : public Graphics::AbstractItem
{
public:
    Hint(const std::string &text = {});

    bool empty() const;

    void setText(const std::string &text);
    void setPosition(const sf::Vector2f pos);
    void reset();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void updateGeometry();

    Graphics::RoundedRectShape _shape;
    sf::Text _text;
};
} // namespace Game
