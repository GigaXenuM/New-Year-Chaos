#pragma once

#include "action/iaction.h"
#include "items/abstractphysicalitem.h"

namespace Game
{

class Bridge : public AbstractPhysicalItem, public IAction
{
public:
    explicit Bridge(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return ItemType::Terrain;
    }
    ActionVariant actionVariant() const override
    {
        return ActionVariant::LowerBridge;
    }

    bool needDestroying() const override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Vector2f position() const override;
    void execute() override;

private:
    bool _needLower{ false };
};

} // namespace Game
