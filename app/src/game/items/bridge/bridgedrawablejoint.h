#pragma once

#include "item/drawable.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Game
{

class Bridge;

class BridgeDrawableJoint : public Graphics::Drawable
{
    friend class Bridge;

public:
    explicit BridgeDrawableJoint(const Bridge *bridge);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void update(float /*deltatime*/) override;

private:
    const Bridge *_bridge{ nullptr };
    sf::Sprite _sprite;
    sf::Sprite _chainSprite;
};

} // namespace Game
