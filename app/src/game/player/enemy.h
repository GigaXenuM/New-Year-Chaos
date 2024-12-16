#pragma once

#include "items/entity/physicalentity.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace Game
{

class Enemy : public PhysicalEntity
{
public:
    Enemy(b2World *world, sf::Shape *shape, const PhysicalEntity *targetEntity, float reloading,
          float bulletPower);

    void updatePhysics() override;

    void damage(float power) override;
    std::vector<AbstractPhysicalItem *> dropLoots() override;

    virtual void updateAnimation(float deltatime)
    {
    }

    void setValue(const float value = 25.f);

protected:
    sf::Sprite _sprite;
    sf::Sprite _healthBar;
    sf::RectangleShape _health;

    const float _scale{ 0.30f };
    const float _scaleHealthBar{ 0.17f };

    sf::Vector2f _healthSize{};

    float _healthPoint{ 100.f };

private:
    void walkingScript();
    void shootingScript();

    const PhysicalEntity *_targetEntity{ nullptr };

    sf::Vector2f _pos;
    const float _moveLimit{ 100.f };
    bool _isAvailableToShoot{ false };
};

} // namespace Game
