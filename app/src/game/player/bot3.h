#pragma once

#include "animation/animation.h"
#include "player/enemy.h"

namespace Game
{

class Bot3 final : public Enemy
{
public:
    Bot3(b2World *world, sf::Shape *shape, const PhysicalEntity *targetEntity);

protected:
    void update(float deltatime) override;
    void updateAnimation(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    std::vector<AbstractPhysicalItem *> dropLoots() override;

private:
    void setupSprites();

    Animation _walkAnimation;
    Animation _deadAnimation;
    Animation _hurtAnimation;
    Animation _throwAnimation;
};

} // namespace Game
