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
constexpr float DEFAULT_MANTADORY_HINT_TIME{ 3 };
} // namespace

Player::Player(b2World *world, sf::Shape *shape)
    : PhysicalEntity(ColliderFactory::create<ItemType::Entity>(world, { shape }), { 5, 45 },
                     std::make_unique<SnowBallGun>(this, world, 1.f, 150.f)),
      _runAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_run),
                     DEFAULT_ANIMATION_FRAME_TIME },
      _deadAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_dead),
                      DEFAULT_ANIMATION_FRAME_TIME },
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_walk),
                      DEFAULT_ANIMATION_FRAME_TIME },
      _idleAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_idle),
                      DEFAULT_ANIMATION_FRAME_TIME },
      _jumpAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_jump) },
      _mandatoryHintTimer{ 0.f, 0.f, DEFAULT_MANTADORY_HINT_TIME }
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

bool Player::isWon() const
{
    return _isWon;
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

    _health.move(-power);
    if (_health.isMin())
        updateState(State::Dead, true);
}

void Player::freezeDamage(float power)
{
    if (isStateActive(State::Dead))
        return;

    float lastValue{ _freeze.get() };
    _freeze.move(-power);

    if (_freeze.isMin())
        damage(lastValue - power);
}

size_t Player::getHealCount() const
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
    freezeDamage(_freeze.get() + 1);
    damage(_health.get() + 1);
    _stamina = 0.f;
}

void Player::setWinStatus(const bool status)
{
    _isWon = status;
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
    case ActionVariant::OpenDoor:
    {
        if (!_hasKey)
            return;
        break;
    }
    case ActionVariant::PickUpTea:
    {
        setMentadoryHint("Q - випити чай, щоб зігрітись");
        ++_countOfHealthItem;
        break;
    }
    case ActionVariant::PickUpKey:
    {
        _hasKey = true;
        break;
    }
    case ActionVariant::MountainObstacle:
        break;
    default:
        assert(false);
        break;
    }

    _availableAction->execute();
}

float Player::getFreezPoints() const
{
    return _freeze.get();
}

float Player::getHealthPoints() const
{
    return _health.get();
}

float Player::getStaminaPoints() const
{
    return _stamina;
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
    if (isStateActive(State::Dead))
        return;
    restoreHealthAndFreezePoints();
    freezeDamage(isStateActive(State::Warming) ? -deltatime * 3 : deltatime);
}

void Player::updateStaminaPoint(float deltatime)
{
    if (_stamina <= 0.f)
        return;
    _stamina -= (deltatime * 10);
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

    _mandatoryHintTimer.move(deltatime);
}

void Player::restoreStaminaPoints(float deltatime)
{
    if (_stamina >= 100.f)
        return;
    _stamina += (deltatime * 10);
}

void Player::restoreHealthAndFreezePoints()
{
    if (isStateActive(State::Dead) || !_isHealthNeeded)
        return;

    _isHealthNeeded = false;

    constexpr float restorationAmount = 25.0f;

    if (!_health.isMax())
    {
        float healthDeficit = 100.0f - _health.get();

        if (healthDeficit < restorationAmount)
        {
            _health.move(healthDeficit);
            _freeze.move(restorationAmount - healthDeficit);
        }
        else
        {
            _health.move(restorationAmount);
        }
    }
    else if (!_freeze.isMax())
    {
        float freezeDeficit = 100.0f - _freeze.get();
        if (freezeDeficit < restorationAmount)
            _freeze.move(freezeDeficit);
        else
            _freeze.move(restorationAmount);
    }
}

std::string Player::hintText(IAction *action)
{
    const bool isTimerFinished{ _mandatoryHintTimer.isMax() };
    const bool needShowMandatoryHint{ !_mandatoryHints.empty() || !isTimerFinished };
    if (needShowMandatoryHint)
    {
        if (!isTimerFinished)
            return _hint.text();

        _mandatoryHintTimer.setMin();

        std::string text{ *_mandatoryHints.begin() };
        _showedMandatoryHints.insert(text);
        _mandatoryHints.erase(_mandatoryHints.cbegin());
        return text;
    }

    if (action == nullptr)
        return {};

    switch (action->actionVariant())
    {
    case ActionVariant::OpenBridge:
    {
        return _hasKey ? action->hintText() : "Знайди ключ, щоб відчинити міст";
    }
    case ActionVariant::OpenDoor:
    {
        return _hasKey ? action->hintText() : "Знайди ключ, щоб відчинити двері";
    }
    case ActionVariant::PickUpTea:
        [[fallthrough]];
    case ActionVariant::PickUpKey:
    case ActionVariant::MountainObstacle:
        return action->hintText();
    }

    assert(false);
    return {};
}

void Player::setMentadoryHint(std::string hintText)
{
    if (!_showedMandatoryHints.contains(hintText))
        _mandatoryHints.insert(hintText);
}

void Player::updatePosition(float deltatime)
{
    if (isStateActive(State::Dead))
    {
        collider()->SetType(b2_staticBody);
        return;
    }
    if (isStateActive(State::Run) && (isStateActive(State::Right) || isStateActive(State::Left)))
    {
        updateStaminaPoint(deltatime);
    }
    else
    {
        restoreStaminaPoints(deltatime);
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
