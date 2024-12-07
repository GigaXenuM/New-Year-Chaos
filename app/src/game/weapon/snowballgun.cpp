#include "snowballgun.h"

#include "items/bullet/snowball.h"
#include "items/entity/physicalentity.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RectangleShape.hpp>

namespace Game
{

SnowBallGun::SnowBallGun(PhysicalEntity *owner, b2World *world)
    : IWeapon(owner, world),
      _icon{ ResourseManager::getInstance()->getTextures(TextureType::SnowBallGun).front() }
{
}

SnowBallGun::~SnowBallGun() = default;

void SnowBallGun::shoot(const sf::Vector2f &startPos, const sf::Vector2f &target)
{
    if (_state.test(State::Reload))
        return;

    _state.set(State::Reload);

    sf::RectangleShape bulletShape{ { 10, 10 } };
    bulletShape.setPosition(Util::pointBy(_owner->boundingRect(), Util::ALIGN_CENTER_STATE));

    auto *bullet{ new SnowBall{ _world, &bulletShape, _owner, SnowBall::Context{ 50.f, target } } };
    bullet->impulse();
    _snowballs.push_back(std::unique_ptr<SnowBall>(bullet));
}

std::optional<float> SnowBallGun::reload() const
{
    return _state.test(State::Reload) ? std::optional<float>{ _reloadTimer } : std::nullopt;
}

void SnowBallGun::update(float deltatime)
{
    updateReloading(deltatime);

    for (std::unique_ptr<PhysicalBullet> &snowball : _snowballs)
    {
        snowball->update(deltatime);
        if (snowball->isStateActive(PhysicalBullet::State::Collide))
        {
            snowball->destroyCollider();
            snowball.reset();
        }
    }

    _snowballs.erase(std::remove_if(_snowballs.begin(), _snowballs.end(),
                                    [](auto &bullet) { return bullet == nullptr; }),
                     _snowballs.end());
}

const std::vector<std::unique_ptr<PhysicalBullet>> &SnowBallGun::bullets() const
{
    return _snowballs;
}

const sf::Texture *SnowBallGun::icon() const
{
    return &_icon;
}

void SnowBallGun::updateReloading(float deltatime)
{
    if (!_state.test(State::Reload))
        return;

    _reloadTimer -= deltatime;
    if (_reloadTimer <= 0.f)
    {
        _reloadTimer = _reloading;
        _state.unset(State::Reload);
    }
}

} // namespace Game
