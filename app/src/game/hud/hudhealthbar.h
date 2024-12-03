#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include <item/abstractitem.h>

namespace Game
{

class HUDHealthBar : public Graphics::AbstractItem
{
public:
    HUDHealthBar(const sf::Texture &iconTexture);

    void setPosition(const sf::Vector2f pos);
    void setIconScaleFactors(float factorX, float factorY);
    void setColor(const sf::Color &color);

    void setValue(const float value);

    [[nodiscard]] const sf::Sprite *getSprite() const;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void setup();
    void setSize();
    void setTextures();

private:
    sf::Sprite _barIcon;
    sf::Sprite _healthBar;
    sf::RectangleShape _health;

    std::pair<float, float> _iconScaleFactors{ 0.02f, 0.02f };
};

} // namespace Game
