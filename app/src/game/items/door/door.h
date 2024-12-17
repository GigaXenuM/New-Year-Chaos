#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "action/iaction.h"
#include "items/abstractphysicalitem.h"
#include "util/limitedvalue.h"

namespace Game
{

class Door : public AbstractPhysicalItem, public IAction
{
public:
    explicit Door(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return _type;
    }
    ActionVariant actionVariant() const override
    {
        return ActionVariant::OpenDoor;
    }

    std::string hintText() const override;

    bool needDestroying() const override;

    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Vector2f position() const override;
    void execute() override;

private:
    ItemType _type{ ItemType::TerrainObstacle };
    sf::Sprite _sprite;
    sf::Sprite _doorSideSprite;
    std::string _hintText;

    Util::LimitedValueF _doorScaling;
};

} // namespace Game
