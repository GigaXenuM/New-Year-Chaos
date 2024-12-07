#pragma once

#include "weapon/IWeapon.h"

#include <SFML/Graphics/Texture.hpp>

#include <vector>

namespace Game
{

class PhysicalEntity;
class PhysicalBullet;

class SnowBallGun : public IWeapon
{
public:
    explicit SnowBallGun(PhysicalEntity *owner, b2World *world);
    ~SnowBallGun() = default;

    void shoot(const sf::Vector2f &startPos, const sf::Vector2f &target) override;
    std::optional<float> reload() const override;

    const sf::Texture *icon() const override;

private:
    sf::Texture _icon;
};

} // namespace Game
