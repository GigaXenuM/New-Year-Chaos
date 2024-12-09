#pragma once

#include "action/iaction.h"
#include "items/abstractphysicalitem.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Game
{

class TeaLoot : public AbstractPhysicalItem, public IAction
{
public:
    TeaLoot(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return ItemType::Loot;
    }
    ActionVariant actionVariant() const override
    {
        return ActionVariant::PickUpTea;
    }

    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Vector2f position() const override;
    void execute() override;

private:
    sf::Sprite _sprite;
};

} // namespace Game
