#pragma once

#include "action/iaction.h"
#include <SFML/Graphics/Sprite.hpp>

#include <items/abstractphysicalitem.h>

namespace Game
{
class WinZone : public AbstractPhysicalItem, public IAction
{
public:
    WinZone(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return ItemType::WinZone;
    }

    ActionVariant actionVariant() const override
    {
        return ActionVariant::FinishGame;
    }

    std::string hintText() const override;

    bool needDestroying() const override;

    virtual sf::Vector2f position() const override
    {
        return Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE);
    }
    virtual void execute() override
    {
    }

protected:
    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::Sprite _sprite;
};

} // namespace Game
