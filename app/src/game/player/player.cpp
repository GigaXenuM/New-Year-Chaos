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
constexpr float MANTADORY_HINT_TIME_DEFINER{ 0.03125 };
constexpr float MIN_MANTADORY_HINT_TIME{ 2 };
constexpr float MAX_MANTADORY_HINT_TIME{ 5 };
} // namespace

Player::Player(b2World *world, sf::Shape *shape, bool menuMode)
    : PhysicalEntity(ColliderFactory::create<ItemType::Entity>(world, { shape }), { 5, 45 },
                     std::make_unique<SnowBallGun>(this, world, 2.f, 25.f)),
      _menuMode{ menuMode },
      _runAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_run),
                     DEFAULT_ANIMATION_FRAME_TIME },
      _deadAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_dead),
                      DEFAULT_ANIMATION_FRAME_TIME },
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_walk),
                      DEFAULT_ANIMATION_FRAME_TIME },
      _idleAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_idle),
                      DEFAULT_ANIMATION_FRAME_TIME },
      _jumpAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_jump) },
      _mandatoryHintTimeDefiner{ MIN_MANTADORY_HINT_TIME, MIN_MANTADORY_HINT_TIME,
                                 MAX_MANTADORY_HINT_TIME },
      _moveSounds{ { SoundAssignment::Walk,
                     sf::Sound{ ResourseManager::getInstance()->getSoundBuffer(SoundType::Walk) } },
                   { SoundAssignment::Run,
                     sf::Sound{
                         ResourseManager::getInstance()->getSoundBuffer(SoundType::Run) } } },
      _pickUpSound{ ResourseManager::getInstance()->getSoundBuffer(SoundType::PickUp) }
{
    _sprite.setScale({ _scale, _scale });
    for (auto &[_, value] : _moveSounds)
        value.setLoop(true);

    if (!_menuMode)
    {
        setMentadoryHint("Чорт... Галімі сніговики вкрали подарунки. Треба повернути!", true);
        setMentadoryHint("[Ф], [В] - переміщення\n[Пробіл] - Стрибнути\n[ЛКМ] - Кинути сніжку",
                         true);
    }

    _pickUpSound.setPitch(1.5f);
}

void Player::restoreFreezePoints()
{
    if (isStateActive(State::Dead) || _countOfTeaItem == 0 || _freeze.isMax())
        return;

    constexpr float restorationAmount = 25.0f;
    float freezeDeficit = 100.0f - _freeze.get();

    if (freezeDeficit < restorationAmount)
        _freeze.move(freezeDeficit);
    else
        _freeze.move(restorationAmount);

    --_countOfTeaItem;
}

void Player::restoreHealthPoints()
{
    if (isStateActive(State::Dead) || _countOfHealthItem <= 0 || _health.isMax())
        return;

    constexpr float restorationAmount = 25.0f;
    const float healthDeficit = 100.0f - _health.get();

    if (healthDeficit < restorationAmount)
        _health.move(healthDeficit);
    else
        _health.move(restorationAmount);

    --_countOfHealthItem;
}

void Player::addTeaItem()
{
    _countOfTeaItem++;
}

void Player::addHealItem()
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

    if (_health.get() < _health.max() / 2.f)
        setMentadoryHint("[А] - юзай аптечку", true);
}

void Player::freezeDamage(float power)
{
    if (isStateActive(State::Dead))
        return;

    float lastValue{ _freeze.get() };
    _freeze.move(-power);

    if (_freeze.get() < _freeze.max() / 2.f)
        setMentadoryHint("[Й] - випити чай, щоб зігрітись", true);

    if (_freeze.isMin())
        damage(power - lastValue);
}

size_t Player::getTeaCount() const
{
    return _countOfTeaItem;
}

size_t Player::getAidKitCount() const
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
    if (isStateActive(State::Dead))
        return;

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

    if (!_menuMode)
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
        if (!_keyState.test(KeyState::BridgeKey))
            return;
        break;
    }
    case ActionVariant::OpenDoor:
    {
        if (!_keyState.test(KeyState::DoorKey))
            return;
        break;
    }
    case ActionVariant::PickUpTea:
    {
        _pickUpSound.play();
        ++_countOfTeaItem;
        break;
    }
    case ActionVariant::PickUpKeyDoor:
    {
        _pickUpSound.play();
        _keyState.set(KeyState::DoorKey);
        break;
    }
    case ActionVariant::PickUpKeyBridge:
    {
        _pickUpSound.play();
        _keyState.set(KeyState::BridgeKey);
        break;
    }
    case ActionVariant::PickUpHealth:
    {
        _pickUpSound.play();
        ++_countOfHealthItem;
        break;
    }
    case ActionVariant::FinishGame:
    {
        setWinStatus(true);
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
    _deadAnimation.stop();

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
    if (isStateActive(State::Dead) || _menuMode)
        return;
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

void Player::updateMoveSound()
{
    const bool isMoved{ isStateActive(State::Right) || isStateActive(State::Left) };
    if (isStateActive(State::OnGround) && isMoved)
    {
        SoundAssignment soundAssignment{ isStateActive(State::Run) ? SoundAssignment::Run
                                                                   : SoundAssignment::Walk };
        for (auto &[key, value] : _moveSounds)
        {
            if (key == soundAssignment)
            {
                if (_currentMoveSound != key)
                {
                    _currentMoveSound = key;
                    value.play();
                }
                continue;
            }
            value.stop();
        }

        return;
    }

    if (_currentMoveSound != SoundAssignment::None)
        for (auto &[_, value] : _moveSounds)
            value.stop();
    _currentMoveSound = SoundAssignment::None;
}

void Player::restoreStaminaPoints(float deltatime)
{
    if (_stamina >= 100.f)
        return;
    _stamina += (deltatime * 10);
}

std::string Player::hintText(IAction *action)
{
    const bool isTimerFinished{ _mandatoryHintTimer.isMax() };
    const bool needShowMandatoryHint{ !_mandatoryHints.empty() || !isTimerFinished };
    if (needShowMandatoryHint)
    {
        if (!isTimerFinished)
            return _hint.text();

        const auto [text, disposable]{ *_mandatoryHints.begin() };
        if (disposable)
        {
            _showedMandatoryHints.insert(text);
            _mandatoryHintTimeDefiner.setValue(MANTADORY_HINT_TIME_DEFINER * text.size());
            _mandatoryHintTimer.changeLimits(_mandatoryHintTimer.min(),
                                             _mandatoryHintTimeDefiner.get());
            _mandatoryHintTimer.setMin();
        }

        _mandatoryHints.erase(_mandatoryHints.cbegin());
        return text;
    }

    if (action == nullptr)
        return {};

    switch (action->actionVariant())
    {
    case ActionVariant::OpenBridge:
    {
        return _keyState.test(KeyState::BridgeKey) ? action->hintText()
                                                   : "Потрібно знайти ключ\n"
                                                     "щоб зняти ланцюги\nта опустити міст";
    }
    case ActionVariant::OpenDoor:
    {
        return _keyState.test(KeyState::DoorKey) ? action->hintText()
                                                 : "Потрібно знайти ключ\nщоб відчинити двері";
    }
    case ActionVariant::PickUpTea:
        [[fallthrough]];
    case ActionVariant::PickUpHealth:
        [[fallthrough]];
    case ActionVariant::PickUpKeyDoor:
    case ActionVariant::PickUpKeyBridge:
    case ActionVariant::MountainObstacle:
        return action->hintText();
        break;
    case ActionVariant::FinishGame:
        return action->hintText();
        break;
    }

    assert(false);
    return {};
}

void Player::setMentadoryHint(std::string hintText, bool disposable)
{
    if (!_showedMandatoryHints.contains(hintText))
        _mandatoryHints.insert({ hintText, disposable });
}

void Player::updatePosition(float deltatime)
{
    const sf::Vector2f playerPos{ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE) };
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setPosition(playerPos);

    if (isStateActive(State::Dead))
        return;

    if (isStateActive(State::Run) && (isStateActive(State::Right) || isStateActive(State::Left)))
    {
        updateStaminaPoint(deltatime);
    }
    else
    {
        restoreStaminaPoints(deltatime);
    }
}

void Player::update(float deltatime)
{
    PhysicalEntity::update(deltatime);

    updatePosition(deltatime);
    updateAnimation(deltatime);
    updateHealthPoint(deltatime);
    updateHint(deltatime);
    updateMoveSound();
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
