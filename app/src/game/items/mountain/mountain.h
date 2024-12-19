#pragma once

#include "action/iaction.h"
#include "items/abstractphysicalitem.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Game
{

class Mountain : public AbstractPhysicalItem, public IAction
{
public:
    explicit Mountain(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return ItemType::NonCollided;
    }
    ActionVariant actionVariant() const override
    {
        return ActionVariant::MountainObstacle;
    }

    std::string hintText() const override;

    bool needDestroying() const override;

    void update(float deltatime) override
    {
    }
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
    }

    sf::Vector2f position() const override;
    void execute() override
    {
    }
};

} // namespace Game
