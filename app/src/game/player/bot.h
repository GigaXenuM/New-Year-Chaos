#pragma once

#include "player/enemy.h"
#include "animation/animation.h"

namespace Game
{

class Bot final : public Enemy
{
public:
    Bot(b2World *world, sf::Shape *shape, const PhysicalEntity *targetEntity);

protected:
    void update(float deltatime) override;
    void updateAnimation(float deltatime) override;
    std::vector<AbstractPhysicalItem *> dropLoots() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void setupSprites();

    Animation _walkAnimation;
    Animation _deadAnimation;
    Animation _hurtAnimation;
    Animation _throwAnimation;
};

} // namespace Game
