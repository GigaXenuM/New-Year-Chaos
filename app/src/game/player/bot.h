#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "animation/animation.h"
#include "items/entity/physicalentity.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Game
{

class Bot : public PhysicalEntity
{
public:
    Bot(b2World *world, sf::Shape *shape);

    void damage(float power) override;
    std::vector<AbstractPhysicalItem *> dropLoots() override;

protected:
    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void setupSprites();

    void walkingScript();
    void shootingScript(float deltatime);

    void updateAnimation(float deltatime);
    void setValue(const float value = 25.f);

    bool _isAvailableToShoot{ false };

    const float _scale{ 0.30f };
    const float _scaleHealthBar{ 0.17f };
    const float _moveLimit{ 100.f };

    float _healthPoint{ 100.f };
    sf::Vector2f _healthSize{};

    sf::Sprite _sprite;
    sf::Sprite _healthBar;
    sf::RectangleShape _health;

    sf::Vector2f _pos{};

    Animation _walkAnimation;
    Animation _deadAnimation;
    Animation _hurtAnimation;
    Animation _throwAnimation;
};

} // namespace Game
