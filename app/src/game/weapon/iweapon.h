#pragma once

#include "iupdatable.h"

#include "util/enumflag.h"

#include <SFML/System/Vector2.hpp>

#include <memory>
#include <optional>
#include <vector>

struct b2World;

namespace sf
{
class Texture;
}

namespace Game
{

class PhysicalBullet;
class PhysicalEntity;

enum class State
{
    Reload,
};

class IWeapon : public IUpdatable
{
public:
    IWeapon(PhysicalEntity *owner, b2World *world, float reloading);
    virtual ~IWeapon();

    void update(float deltatime) override;
    virtual const sf::Texture *icon() const = 0;
    virtual std::optional<float> reload() const = 0;
    virtual const std::vector<std::unique_ptr<PhysicalBullet>> &bullets() const;
    virtual void shoot(const sf::Vector2f &startPos, const sf::Vector2f &target) = 0;

protected:
    virtual void updateReloading(float deltatime);

protected:
    b2World *_world{ nullptr };
    PhysicalEntity *_owner{ nullptr };
    Util::EnumFlag<State> _state;

    float _reloading{ 3.f };
    float _reloadTimer{ 3.f };

    std::vector<std::unique_ptr<PhysicalBullet>> _bullets;
};

} // namespace Game
