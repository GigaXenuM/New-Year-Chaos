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

namespace
{
constexpr float DEFAULT_ANIMATION_FRAME_TIME{ 0.075 };
}

Player *gPlayer = nullptr;

Player::Player(b2World *world, sf::Shape *shape)
    : PhysicalEntity(ColliderFactory::create<ItemType::Entity>(world, { shape }), { 5, 45 },
                     std::make_unique<SnowBallGun>(this, world)),
      _runAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_run),
                     DEFAULT_ANIMATION_FRAME_TIME },
      _deadAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_dead),
                      DEFAULT_ANIMATION_FRAME_TIME },
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_walk),
                      DEFAULT_ANIMATION_FRAME_TIME },
      _idleAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_idle),
                      DEFAULT_ANIMATION_FRAME_TIME },
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

const Graphics::Drawable *Player::hint() const
{
    return &_hint;
}

void Player::damage(float power)
{
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

    constexpr float threshold{ 100.f };
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

    _hint.setText(hintText(_availableAction));
}

void Player::executeAvailableAction()
{
    if (_availableAction == nullptr)
        return;

    switch (_availableAction->actionVariant())
    {
    case ActionVariant::OpenBridge:
    {
        if (!_hasKey)
            return;
        break;
    }
    case ActionVariant::PickUpTea:
    {
        ++_countOfHealthItem;
        break;
    }
    case ActionVariant::PickUpKey:
        _hasKey = true;
        break;
    }

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

    const bool isMoved{ isStateActive(State::Right) || isStateActive(State::Left) };
    if (isMoved)
    {
        Animation &animation{ isStateActive(State::Run) ? _runAnimation : _walkAnimation };

        animation.start(deltatime, _sprite);
        const float direction{ isStateActive(State::Right) ? _scale : -_scale };
        _sprite.setScale({ direction, _scale });
        return;
    }

    _idleAnimation.start(deltatime, _sprite);
}

void Player::updateHealthPoint(float deltatime)
{
    restoreHealthAndFreezePoints();
    damage(deltatime);
}

void Player::updateHint(float deltatime)
{
    if (!_hint.empty())
    {
        const sf::Vector2f topRectPos{ Util::pointBy(boundingRect(), { Align::Top }) };
        const sf::Vector2f offset{ 0, -boundingRect().height / 2.f };
        _hint.setPosition(topRectPos + offset);
        _hint.update(deltatime);
    }
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

std::string Player::hintText(IAction *action) const
{
    if (action == nullptr)
        return {};

    switch (action->actionVariant())
    {
    case ActionVariant::OpenBridge:
    {
        return _hasKey ? action->hintText() : "Find a key\nto unlock";
    }
    case ActionVariant::PickUpTea:
        [[fallthrough]];
    case ActionVariant::PickUpKey:
        return action->hintText();
    }

    assert(false);
    return {};
}

void Player::updatePosition()
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

    updatePosition();
    updateAnimation(deltatime);
    updateHealthPoint(deltatime);
    updateHint(deltatime);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);

    for (const std::unique_ptr<PhysicalBullet> &bullet : weapon()->bullets())
        target.draw(*bullet, states);

    if (!_hint.empty())
        _hint.draw(target, states);
}

} // namespace Game
