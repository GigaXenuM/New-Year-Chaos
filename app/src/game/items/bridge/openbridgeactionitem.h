#pragma once

#include "action/iaction.h"
#include "items/abstractphysicalitem.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Game
{

class Bridge;

class OpenBridgeActionItem : public AbstractPhysicalItem, public IAction
{
public:
    OpenBridgeActionItem(b2World *world, sf::Shape *shape, Bridge *bridge);

    ItemType type() const override
    {
        return ItemType::NonCollided;
    }
    ActionVariant actionVariant() const override
    {
        return ActionVariant::OpenBridge;
    }

    std::string hintText() const override;

    bool needDestroying() const override;

    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Vector2f position() const override;
    void execute() override;

private:
    Bridge *_bridge{ nullptr };
    sf::Sprite _sprite;
    std::string _hintText;
};

} // namespace Game
