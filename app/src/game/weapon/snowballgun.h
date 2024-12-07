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
    enum class State
    {
        Reload,
    };

    explicit SnowBallGun(PhysicalEntity *owner, b2World *world);
    ~SnowBallGun();

    void shoot(const sf::Vector2f &startPos, const sf::Vector2f &target) override;
    std::optional<float> reload() const override;
    void update(float deltatime) override;

    const std::vector<std::unique_ptr<PhysicalBullet>> &bullets() const override;
    const sf::Texture &icon() const override;

private:
    void updateReloading(float deltatime);

    PhysicalEntity *_owner{ nullptr };
    b2World *_world{ nullptr };
    sf::Texture _icon;

    const float _reloading{ 3.f };
    float _reloadTimer{ _reloading };

    std::vector<std::unique_ptr<PhysicalBullet>> _snowballs;

    Util::EnumFlag<State> _state;
};

} // namespace Game
