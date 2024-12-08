#include "player.h"

#include "items/bullet/physicalbullet.h"
#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"
#include "util/geometryoperation.h"
#include "weapon/IWeapon.h"
#include "weapon/snowballgun.h"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <box2d/b2_world.h>

namespace Game
{

Player *gPlayer = nullptr;

Player::Player(b2World *world, sf::Shape *shape)
    : PhysicalEntity(ColliderFactory::create<ItemType::Entity>(world, { shape }), { 5, 30 },
                     std::make_unique<SnowBallGun>(this, world)),
      _runAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_run) },
      _deadAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_dead) },
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_walk) },
      _idleAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_idle) },
      _jumpAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_jump) }
{
    _sprite.setScale({ _scale, _scale });
}

void Player::health()
{
    if (_countOfHealthItem <= 0)
        return;

    _needHealth = true;
    --_countOfHealthItem;
}

void Player::addHealthItem()
{
    _countOfHealthItem++;
}

sf::Vector2f Player::getPosition() const
{
    return _sprite.getPosition();
}

void Player::damage(float power)
{
    if (isStateActive(State::Dead))
        return;

    if (_freezPoint > 0.f)
    {
        _freezPoint -= power;
        if (_freezPoint <= 0.f)
            _freezPoint = 0.f;
    }
    else if (_healthPoint > 0)
    {
        _healthPoint -= power;
        if (_healthPoint <= 0.f)
        {
            _healthPoint = 0.f;
            updateState(State::Dead, true);
        }
    }
}

size_t Player::getHealthCount() const
{
    return _countOfHealthItem;
}

bool Player::isDead() const
{
    return isStateActive(State::Dead) && _deadAnimation.isFinished();
}

float Player::getFreezPoints() const
{
    return _freezPoint;
}

float Player::getHealthPoints() const
{
    return _healthPoint;
}

void Player::updateAnimation(float deltatime)
{
    if (isStateActive(State::Dead))
    {
        if (!_deadAnimation.isFinished())
            _deadAnimation.start(deltatime, _sprite, false);

        return;
    }

    if (isStateActive(State::Run))
    {
        _runAnimation.start(deltatime, _sprite);
        if (isStateActive(State::Right))
            _sprite.setScale({ _scale, _scale });
        else if (isStateActive(State::Left))
            _sprite.setScale({ -_scale, _scale });
        return;
    }

    const bool isMoved{ isStateActive(State::Right) || isStateActive(State::Left) };
    if (!isMoved)
    {
        _idleAnimation.start(deltatime, _sprite);
        return;
    }

    _walkAnimation.start(deltatime, _sprite);

    if (isStateActive(State::Right))
        _sprite.setScale({ _scale, _scale });
    else if (isStateActive(State::Left))
        _sprite.setScale({ -_scale, _scale });
}

void Player::updateHealthPoint(float deltatime)
{
    tryToRestoreHealthPoint();
    damage(deltatime);
}

void Player::tryToRestoreHealthPoint()
{
    if (!_needHealth || _deadAnimation.isPlaying())
        return;

    _needHealth = false;

    const float restorationPoints = 25.f;

    if (_healthPoint < 100.f)
    {
        if ((100.f - _healthPoint) < restorationPoints)
            _healthPoint += (100.f - _healthPoint);
        else
            _healthPoint += restorationPoints;
    }
    else if (_freezPoint < 100.f)
    {
        if ((100.f - _freezPoint) < restorationPoints)
            _freezPoint += (100.f - _freezPoint);
        else
            _freezPoint += restorationPoints;
    }
}

void Player::updatePosition(float deltatime)
{
    const sf::Vector2f playerPos{ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE) };
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setPosition(playerPos);
}

void Player::update(float deltatime)
{
    PhysicalEntity::update(deltatime);

    updatePosition(deltatime);
    updateAnimation(deltatime);
    updateHealthPoint(deltatime);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);

    for (const auto &bullet : weapon()->bullets())
        target.draw(*bullet, states);

    sf::RectangleShape border(_sprite.getLocalBounds().getSize());
    border.setPosition(_sprite.getPosition());
    border.setOutlineColor(isStateActive(State::OnGround) ? sf::Color::Green : sf::Color::Red);
    border.setOutlineThickness(10);
    border.setFillColor(sf::Color::Transparent);
    border.setOrigin(_sprite.getOrigin());
    border.setScale(_sprite.getScale());
    target.draw(border, states);
}

} // namespace Game
