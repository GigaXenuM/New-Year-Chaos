#include "bot2.h"

#include "player.h"

#include "items/bullet/physicalbullet.h"
#include "items/colliderfactory.h"
#include "items/loot/keyloot.h"
#include "weapon/snowballgun.h"

#include "resources/resourcemanager.h"
#include "util/geometryoperation.h"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <box2d/b2_world.h>

namespace Game
{

Bot2::Bot2(b2World *world, sf::Shape *shape)
    : PhysicalEntity(ColliderFactory::create<ItemType::Entity>(world, { shape }), { 5, 30 },
                     std::make_unique<SnowBallGun>(this, world, 40.f)),
      _pos{ boundingRect().getPosition() },
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Snowman_2_walk) },
      _deadAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Snowman_2_dead) },
      _hurtAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Snowman_2_hurt) },
      _throwAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Snowman_2_throw) }
{
    setupSprites();
}

void Bot2::damage(float power)
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

std::vector<AbstractPhysicalItem *> Bot2::dropLoots()
{
    sf::RectangleShape shape{ { 50, 50 } };
    shape.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));
    return std::vector<AbstractPhysicalItem *>{ new KeyLoot(collider()->GetWorld(), &shape) };
}

void Bot2::update(float deltatime)
{
    PhysicalEntity::update(deltatime);

    if (isStateActive(State::Dead) && _deadAnimation.isFinished())
        updateState(State::RemoveMe, true);

    walkingScript();
    shootingScript(deltatime);

    updateAnimation(deltatime);
}

void Bot2::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const std::unique_ptr<PhysicalBullet> &bullet : weapon()->bullets())
        target.draw(*bullet, states);

    target.draw(_sprite, states);
    target.draw(_healthBar, states);
    target.draw(_health, states);
}

void Bot2::setupSprites()
{
    updateState(State::Right, true);

    _sprite.setScale({ _scale, _scale });
    _healthBar.setScale({ _scaleHealthBar, _scaleHealthBar });

    _healthBar.setTexture(
        ResourseManager::getInstance()->getTextures(TextureType::Viking_health_bar)[0]);

    _health.setFillColor(sf::Color::Yellow);
    _health.setScale({ _scaleHealthBar, _scaleHealthBar });
    _health.setSize(
        { _healthBar.getLocalBounds().width * 0.9f, _healthBar.getLocalBounds().height * 0.35f });
    _healthSize = _health.getSize();
}

void Bot2::updateAnimation(float deltatime)
{
    if (isStateActive(State::Dead))
    {
        _deadAnimation.start(deltatime, _sprite, false);
        return;
    }
    const bool isMoved{ isStateActive(State::Right) || isStateActive(State::Left) };
    if (isMoved)
    {
        if (isStateActive(State::Right))
            _sprite.setScale({ _scale, _scale });
        else if (isStateActive(State::Left))
            _sprite.setScale({ -_scale, _scale });
        _walkAnimation.start(deltatime, _sprite);
        return;
    }
}

void Bot2::setValue(const float value)
{
    if (value < 0.0f || value > 100.f)
        return;
    _health.setSize({
        _healthSize.x * value / 100,
        _healthSize.y,
    });
}

void Bot2::walkingScript()
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

void Bot2::shootingScript(float deltatime)
{
    if (!_isAvailableToShoot)
        return;

    shoot(gPlayer->getPosition());
}

} // namespace Game
