#pragma once

#include "item/abstractitem.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Game
{
class WeaponIcon : public Graphics::AbstractItem
{
public:
    WeaponIcon(const sf::Vector2f &size);

    void setPosition(const sf::Vector2f pos);
    [[nodiscard]] const sf::Sprite *getSprite() const;

    sf::FloatRect globalRect() const override;
    sf::FloatRect localRect() const override;
    sf::Vector2f position() const override;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    mutable sf::Sprite _icon;
    mutable sf::Text _text;

    const sf::Vector2f _size;
};

} // namespace Game
