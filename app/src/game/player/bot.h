#pragma once

#include "IBot.h"
#include "animation/animation.h"

namespace Game
{

class Bot final : public IBot
{
public:
    Bot(b2World *world, sf::Shape *shape);

protected:
    void update(float deltatime) override;
    void updateAnimation(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void setupSprites();

    Animation _walkAnimation;
    Animation _deadAnimation;
    Animation _hurtAnimation;
    Animation _throwAnimation;
};

} // namespace Game
