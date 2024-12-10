#include "iweapon.h"

#include "items/bullet/physicalbullet.h"
#include "items/entity/physicalentity.h"

namespace Game
{

IWeapon::IWeapon(PhysicalEntity *owner, b2World *world) : _world{ world }, _owner{ owner }
{
}

IWeapon::~IWeapon() = default;

void IWeapon::update(float deltatime)
{
    updateReloading(deltatime);
    for (std::unique_ptr<PhysicalBullet> &snowball : _bullets)
    {
        snowball->update(deltatime);
        if (snowball->needDestroying())
        {
            snowball->destroyCollider();
            snowball.reset();
        }
    }

    _bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(),
                                  [](auto &bullet) { return bullet == nullptr; }),
                   _bullets.end());
}

const std::vector<std::unique_ptr<PhysicalBullet>> &IWeapon::bullets() const
{
    return _bullets;
}

void IWeapon::updateReloading(float deltatime)
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
