#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include <items/abstractphysicalitem.h>

namespace Game
{
class WinZone : public AbstractPhysicalItem
{
public:
    WinZone(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return ItemType::WinZone;
    }
    bool needDestroying() const override;

protected:
    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::Sprite _sprite;
};

} // namespace Game
