#pragma once

#include "weapon/iweapon.h"

#include <SFML/Graphics/Texture.hpp>

namespace Game
{

class PhysicalEntity;
class PhysicalBullet;

class SnowBallGun : public IWeapon
{
public:
    explicit SnowBallGun(PhysicalEntity *owner, b2World *world, float reloading, float power);
    ~SnowBallGun() = default;

    void shoot(const sf::Vector2f &startPos, const sf::Vector2f &target) override;
    std::optional<float> reload() const override;

    const sf::Texture *icon() const override;

private:
    sf::Texture _icon;
    float _power;
};

} // namespace Game
