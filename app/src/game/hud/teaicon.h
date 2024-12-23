#pragma once

#include "SFML/Graphics/Text.hpp"
#include <SFML/Graphics/Sprite.hpp>

#include <item/abstractitem.h>

namespace Game
{
class TeaIcon : public Graphics::AbstractItem
{
public:
    TeaIcon();

    sf::FloatRect globalRect() const override;

    void setTexture(sf::Texture &texture);

    void updateHealCount(size_t healCount);
    void setScaleFactor(float x, float y);

    void setPosition(const sf::Vector2f pos);
    [[nodiscard]] const sf::Sprite *getSprite() const;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::Text _text;
    sf::Sprite _teaIcon;
};

} // namespace Game
