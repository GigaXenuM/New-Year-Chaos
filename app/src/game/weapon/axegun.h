#pragma once

#include "iweapon.h"

#include "items/entity/physicalentity.h"

namespace Game
{
class PhysicalBullet;

class AxeGun : public IWeapon
{
public:
    AxeGun(PhysicalEntity *owner, b2World *world);
    ~AxeGun() = default;

protected:
    void shoot(const sf::Vector2f &startPos, const sf::Vector2f &target) override;
    std::optional<float> reload() const override;

    const sf::Texture *icon() const override;
};
} // namespace Game
