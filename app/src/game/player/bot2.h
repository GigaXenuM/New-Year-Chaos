#pragma once

#include "animation/animation.h"
#include "ibot.h"

namespace Game
{

class Bot2 final : public IBot
{
public:
    Bot2(b2World *world, sf::Shape *shape);

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
