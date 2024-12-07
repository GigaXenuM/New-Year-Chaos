#pragma once

#include "weapon/iweapon.h"

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
    ~SnowBallGun();

    void shoot(const sf::Vector2f &startPos, const sf::Vector2f &target) override;
    std::optional<float> reload() const override;
    void update(float deltatime) override;

    const std::vector<std::unique_ptr<PhysicalBullet>> &bullets() const override;
    const sf::Texture *icon() const override;

private:
    void updateReloading(float deltatime);

    sf::Texture _icon;

    std::vector<std::unique_ptr<PhysicalBullet>> _snowballs;
};

} // namespace Game
