#include "ibot.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "items/loot/keyloot.h"
#include "player.h"
#include "weapon/iweapon.h"

namespace Game
{
class PhysicalBullet;

IBot::IBot(b2Body *collider, const Context &context, std::unique_ptr<IWeapon> weapon)
    : PhysicalEntity(collider, context, std::move(weapon)), _pos{ boundingRect().getPosition() }
{
}

void IBot::damage(float power)
{
    if (isStateActive(State::Dead))
        return;

    _healthPoint -= power;

    if (_healthPoint <= 0.f)
    {
        _healthPoint = 0.f;
        updateState(State::Dead, true);
    }
    setValue(_healthPoint);
}

std::vector<AbstractPhysicalItem *> IBot::dropLoots()
{
    sf::RectangleShape shape{ { 50, 50 } };
    shape.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));
    return std::vector<AbstractPhysicalItem *>{ new KeyLoot(collider()->GetWorld(), &shape) };
}

void IBot::setValue(const float value)
{
    if (value < 0.0f || value > 100.f)
        return;
    _health.setSize({
        _healthSize.x * value / 100,
        _healthSize.y,
    });
}
void IBot::walkingScript()
{
    if (isStateActive(State::Dead))
        return;

    const auto healthBarSpriteHeight = _healthBar.getGlobalBounds().height;
    const sf::ConvexShape shape{ Util::convertBodyToSFMLShape(collider()) };
    const sf::Vector2f playerPos{ Util::pointBy(shape.getLocalBounds(), Util::ALIGN_CENTER_STATE) };
    const sf::Vector2f healthBarPos = { playerPos.x - _healthBar.getGlobalBounds().width / 2,
                                        playerPos.y - (healthBarSpriteHeight * 4) };
    const sf::Vector2f healthPos = { playerPos.x, playerPos.y - healthBarSpriteHeight * 4 };

    const bool isRightEnd{ playerPos.x >= _pos.x + _moveLimit };
    const bool isLeftEnd{ playerPos.x <= _pos.x - _moveLimit };

    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));

    if (isLeftEnd)
    {
        updateState(State::Right, true);
        updateState(State::Left, false);
    }
    else if (isRightEnd)
    {
        updateState(State::Left, true);
        updateState(State::Right, false);
    }

    _sprite.setPosition(playerPos);

    _healthBar.setOrigin(Util::pointBy(_healthBar.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _healthBar.setPosition({ healthBarPos.x, healthBarPos.y - 15 });

    _healthBar.setOrigin(Util::pointBy(_healthBar.getLocalBounds(), Align::Left));
    _health.setPosition({ _healthBar.getPosition().x + 6, _healthBar.getPosition().y - 4 });

    if (gPlayer->getPosition().x >= _pos.x - (_moveLimit * 5)
        && gPlayer->getPosition().x <= _pos.x + (_moveLimit * 5))
        _isAvailableToShoot = true;
    else
        _isAvailableToShoot = false;
}

void IBot::shootingScript(float deltatime)
{
    if (!_isAvailableToShoot)
        return;

    shoot(gPlayer->getPosition());
}
} // namespace Game