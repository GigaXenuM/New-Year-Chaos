#include "axegun.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "items/bullet/AxeBullet.h"

namespace Game
{
AxeGun::AxeGun(PhysicalEntity *owner, b2World *world) : IWeapon(owner, world)
{
}

void AxeGun::shoot(const sf::Vector2f &startPos, const sf::Vector2f &target)
{
    if (_state.test(State::Reload))
        return;

    _state.set(State::Reload);

    sf::RectangleShape bulletShape{ { 10, 10 } };
    bulletShape.setPosition(Util::pointBy(_owner->boundingRect(), Util::ALIGN_CENTER_STATE));

    auto *bullet{ new AxeBullet{ _world, &bulletShape, _owner,
                                 AxeBullet::Context{ 50.f, target } } };
    bullet->impulse();
    _axes.push_back(std::unique_ptr<AxeBullet>(bullet));
}

std::optional<float> AxeGun::reload() const
{
    return std::nullopt;
}

void AxeGun::update(float deltatime)
{
}

const std::vector<std::unique_ptr<PhysicalBullet>> &AxeGun::bullets() const
{
    return _axes;
}

const sf::Texture *AxeGun::icon() const
{
    return nullptr;
}

} // namespace Game