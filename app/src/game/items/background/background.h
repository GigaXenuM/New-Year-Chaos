#pragma once

#include "item/drawable.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>

namespace sf
{
class View;
}

namespace Game
{

class Background : public Graphics::Drawable
{
public:
    Background(const sf::View *view);

    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    struct ParallaxLayer
    {
        explicit ParallaxLayer(const sf::Texture &texture, float speed)
            : sprite{ texture }, speed{ speed }
        {
        }

        sf::Sprite sprite;
        float speed{ 0.f };
        sf::Vector2f offset;
    };

    void updateSprites();

    const sf::View *_view{ nullptr };

    const sf::Vector2f _startViewPos;
    std::array<ParallaxLayer, 4> _layers;
};

} // namespace Game
