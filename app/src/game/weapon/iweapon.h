#pragma once

#include "iupdatable.h"

#include "items/bullet/physicalbullet.h"

#include <SFML/System/Vector2.hpp>

#include <memory>
#include <optional>
#include <vector>

namespace Game
{
class IWeapon : public IUpdatable
{
public:
    ~IWeapon() = default;

    virtual void shoot(const sf::Vector2f &startPos, const sf::Vector2f &target) = 0;

    virtual std::optional<float> reload() const = 0;

    virtual const std::vector<std::unique_ptr<PhysicalBullet>> &bullets() const = 0;
    virtual const sf::Texture &icon() const = 0;
};

} // namespace Game
