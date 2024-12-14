#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <item/abstractitem.h>

namespace Game
{
class Hint : public Graphics::AbstractItem
{
public:
    Hint(const std::string &text);

    void setPosition(const sf::Vector2f pos);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::RectangleShape _shape;
    sf::Text _text;
};
} // namespace Game
