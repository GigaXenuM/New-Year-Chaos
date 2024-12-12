#include "player.h"

#include "action/iaction.h"
#include "items/bullet/physicalbullet.h"
#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"
#include "util/geometryoperation.h"
#include "weapon/iweapon.h"
#include "weapon/snowballgun.h"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <box2d/b2_world.h>

namespace Game
{

Player *gPlayer = nullptr;

Player::Player(b2World *world, sf::Shape *shape)
    : PhysicalEntity(ColliderFactory::create<ItemType::Entity>(world, { shape }), { 5, 45 },
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

    _isHealthNeeded = true;
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
    // power *= 5;
    if (isStateActive(State::Dead))
        return;

    if (_freeze > 0.f)
    {
        _freeze -= power;
        if (_freeze <= 0.f)
            _freeze = 0.f;
    }
    if (_health > 0 && _freeze < 0.01f)
    {
        _health -= power;
        if (_health <= 0.f)
        {
            _health = 0.f;
            updateState(State::Dead, true);
        }
    }
}

size_t Player::getHealthCount() const
{
    return _countOfHealthItem;
}

sf::Sprite &Player::getSprite()
{
    return _sprite;
}

bool Player::isDead() const
{
    return isStateActive(State::Dead) && _deadAnimation.isFinished();
}

void Player::kill()
{
    damage(_health + _freeze + 1);
}

void Player::visitActions(const std::vector<IAction *> &actions)
{
    _availableAction = nullptr;

    constexpr float threshold{ 200.f };
    float smallestDistance{ threshold };
    for (auto *action : actions)
    {
        const float currentDistance{ Util::distance(
            Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE) - action->position()) };
        if (currentDistance < threshold && currentDistance < smallestDistance)
        {
            smallestDistance = currentDistance;
            _availableAction = action;
        }
    }
}

void Player::executeAvailableAction()
{
    if (!_availableAction)
        return;

    switch (_availableAction->actionVariant())
    {
    case ActionVariant::PickUpTea:
        ++_countOfHealthItem;
        break;
    default:
        break;
    }

    if (_availableAction != nullptr)
        _availableAction->execute();
}

float Player::getFreezPoints() const
{
    return _freeze;
}

float Player::getHealthPoints() const
{
    return _health;
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
    restoreHealthAndFreezePoints();
    damage(deltatime);
}

void Player::restoreHealthAndFreezePoints()
{
    if (!_isHealthNeeded || _deadAnimation.isPlaying())
        return;

    _isHealthNeeded = false;

    constexpr float restorationAmount = 25.0f;

    if (_health < 100.0f)
    {
        float healthDeficit = 100.0f - _health;

        if (healthDeficit < restorationAmount)
        {
            _health += healthDeficit;
            _freeze += restorationAmount - healthDeficit;
        }
        else
        {
            _health += restorationAmount;
        }
    }
    else if (_freeze < 100.0f)
    {
        float freezeDeficit = 100.0f - _freeze;

        if (freezeDeficit < restorationAmount)
        {
            _freeze += freezeDeficit;
        }
        else
        {
            _freeze += restorationAmount;
        }
    }
}

void Player::updatePosition(float deltatime)
{
    if (isStateActive(State::Dead))
    {
        collider()->SetType(b2_staticBody);
        return;
    }
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

    for (const std::unique_ptr<PhysicalBullet> &bullet : weapon()->bullets())
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
