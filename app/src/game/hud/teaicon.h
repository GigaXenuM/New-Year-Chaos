#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include <item/abstractitem.h>

namespace Game
{
class TeaIcon : public Graphics::AbstractItem
{
public:
    TeaIcon();

    void setPosition(const sf::Vector2f pos);
    [[nodiscard]] const sf::Sprite *getSprite() const;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::Sprite _teaIcon;
};

} // namespace Game
