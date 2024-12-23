#include "enemy.h"

#include "items/colliderfactory.h"
#include "weapon/snowballgun.h"

namespace Game
{

Enemy::Enemy(b2World *world, sf::Shape *shape, const PhysicalEntity *targetEntity, float reloading,
             float bulletPower)
    : PhysicalEntity(ColliderFactory::create<ItemType::Entity>(world, { shape }), { 5, 30 },
                     std::make_unique<SnowBallGun>(this, world, reloading, bulletPower)),
      _targetEntity{ targetEntity },
      _pos{ boundingRect().getPosition() }
{
}

void Enemy::updatePhysics()
{
    walkingScript();
    shootingScript();

    PhysicalEntity::updatePhysics();
}

void Enemy::damage(float power)
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

void Enemy::setValue(const float value)
{
    if (value < 0.0f || value > 100.f)
        return;
    _health.setSize({
        _healthSize.x * value / 100,
        _healthSize.y,
    });
}

void Enemy::walkingScript()
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

    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));

    _healthBar.setOrigin(Util::pointBy(_healthBar.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _healthBar.setPosition({ healthBarPos.x, healthBarPos.y - 15 });

    _healthBar.setOrigin(Util::pointBy(_healthBar.getLocalBounds(), Align::Left));
    _health.setPosition({ _healthBar.getPosition().x + 6, _healthBar.getPosition().y - 4 });

    sf::Vector2f targetPos{ Util::pointBy(_targetEntity->boundingRect(),
                                          Util::ALIGN_CENTER_STATE) };

    if (targetPos.x >= _pos.x - (_moveLimit * 7) && targetPos.x <= _pos.x + (_moveLimit * 7))
        _isAvailableToShoot = true;
    else
        _isAvailableToShoot = false;
}

void Enemy::shootingScript()
{
    if (!_isAvailableToShoot)
        return;

    sf::Vector2f playerPos{ Util::pointBy(_targetEntity->boundingRect(),
                                          Util::ALIGN_CENTER_STATE) };
    sf::Vector2f currentPos{ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE) };
    float distance{ Util::distance(playerPos - currentPos) };
    sf::Vector2f targetPos{ playerPos - sf::Vector2f{ 0, distance / 2 } };
    shoot(targetPos);
}

} // namespace Game
