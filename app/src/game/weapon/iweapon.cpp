#include "IWeapon.h"

namespace Game
{

IWeapon::IWeapon(PhysicalEntity *owner, b2World *world) : _world{ world }, _owner{ owner }
{
}

void IWeapon::update(float deltatime)
{
    updateReloading(deltatime);
    for (std::unique_ptr<PhysicalBullet> &snowball : _bollets)
    {
        snowball->update(deltatime);
        if (snowball->isStateActive(PhysicalBullet::State::Collide))
        {
            snowball->destroyCollider();
            snowball.reset();
        }
    }

    _bollets.erase(std::remove_if(_bollets.begin(), _bollets.end(),
                                  [](auto &bullet) { return bullet == nullptr; }),
                   _bollets.end());
}

const std::vector<std::unique_ptr<PhysicalBullet>> &IWeapon::bullets() const
{
    return _bollets;
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