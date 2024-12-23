#pragma once

#include "action/iaction.h"
#include "items/abstractphysicalitem.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Game
{
class HealthLoot : public AbstractPhysicalItem, public IAction
{
public:
    HealthLoot(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return ItemType::Loot;
    }
    ActionVariant actionVariant() const override
    {
        return ActionVariant::PickUpHealth;
    }

    std::string hintText() const override;

    bool needDestroying() const override;
    void prepareDestroy();

    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Vector2f position() const override;
    void execute() override;

private:
    sf::Sprite _sprite;
    float _needDestroy{ false };
};
} // namespace Game
