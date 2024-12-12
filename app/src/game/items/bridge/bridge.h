#pragma once

#include "action/iaction.h"
#include "items/abstractphysicalitem.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Game
{

class Bridge : public AbstractPhysicalItem, public IAction
{
public:
    explicit Bridge(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return _type;
    }
    ActionVariant actionVariant() const override
    {
        return ActionVariant::LowerBridge;
    }

    bool needDestroying() const override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void update(float deltatime) override;

    sf::Vector2f position() const override;
    void execute() override;

private:
    sf::Sprite _sprite;
    sf::Sprite _chainSprite;
    ItemType _type{ ItemType::TerrainObstacle };
};

} // namespace Game
