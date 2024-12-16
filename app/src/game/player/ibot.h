#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "items/entity/physicalentity.h"

namespace Game
{
class IBot : public PhysicalEntity
{
public:
    IBot(b2Body *collider, const Context &context, std::unique_ptr<IWeapon> weapon);

    void damage(float power) override;
    std::vector<AbstractPhysicalItem *> dropLoots() override;

    virtual void updateAnimation(float deltatime)
    {
    }

    void setValue(const float value = 25.f);

protected:
    void walkingScript();
    void shootingScript(float deltatime);

protected:
    sf::Sprite _sprite;
    sf::Sprite _healthBar;
    sf::RectangleShape _health;

    const float _scale{ 0.30f };
    const float _scaleHealthBar{ 0.17f };
    const float _moveLimit{ 100.f };

    sf::Vector2f _pos{};
    sf::Vector2f _healthSize{};

    float _healthPoint{ 100.f };
    bool _isAvailableToShoot{ false };
};
} // namespace Game
