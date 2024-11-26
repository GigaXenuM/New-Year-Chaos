#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include <item/abstractitem.h>

namespace sf
{
class View;
}

class HUDHealthBar : public Graphics::AbstractItem
{
public:
    HUDHealthBar(sf::View *view);

protected:
    void update(float deltatime) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void setup();

    void setSize();
    void setTextures();
    void setPosition();

private:
    sf::View *_gameView{ nullptr };

    sf::Sprite _freezBarSprite;
    sf::Sprite _healthBarSprite;
};
