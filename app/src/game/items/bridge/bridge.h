#pragma once

#include "items/abstractphysicalitem.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Game
{
class BridgeDrawableJoint;

class Bridge : public AbstractPhysicalItem
{
    friend class BridgeDrawableJoint;
    friend class OpenBridgeActionItem;

public:
    explicit Bridge(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return _type;
    }

    bool needDestroying() const override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void update(float deltatime) override;

    Graphics::Drawable *postDrawElement() const;

private:
    const float _rotationOffset{ -90.f };
    sf::Sprite _sprite;
    ItemType _type{ ItemType::TerrainObstacle };

    BridgeDrawableJoint *_postDrawElement{ nullptr };
    mutable sf::Vector2f _actionPos;
};

} // namespace Game
