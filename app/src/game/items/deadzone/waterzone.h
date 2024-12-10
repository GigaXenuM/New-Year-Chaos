#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "animation/animation.h"
#include "items/abstractphysicalitem.h"

namespace Game
{

class DeadWaterZone : public AbstractPhysicalItem
{
public:
    DeadWaterZone(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return ItemType::WaterZone;
    }

    void playAnimation();

protected:
    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    bool _isNeedPlayAnim{ false };

    Animation _divingAnimation;
    sf::Sprite _sprite;
};

} // namespace Game
