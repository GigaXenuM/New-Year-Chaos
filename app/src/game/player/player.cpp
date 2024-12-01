#include "player.h"

#include "resources/resourcemanager.h"
#include "util/geometryoperation.h"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

Player *gPlayer = nullptr;

Player::Player(b2Body *collider)
    : PhysicalEntity(collider, { 5, 30 }),
      _runAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_run) },
      _deadAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_dead) },
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_walk) },
      _idleAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_idle) },
      _jumpAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_jump) }
{
    _sprite.setScale({ _scale, _scale });
}

sf::Vector2f Player::getPosition() const
{
    return _sprite.getPosition();
}

bool Player::isDead() const
{
    return _healthPoint == 0.f && _deadAnimation.isFinished();
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
    if (_healthPoint == 0.f)
    {
        if (!_deadAnimation.isFinished())
            _deadAnimation.start(deltatime, _sprite, false);

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
    _healthUpdateTimer += deltatime;

    if (_healthUpdateTimer >= _healthUpdateInterval)
    {
        _healthUpdateTimer = 0.0f; // reset timer
        if (_freezPoint > 0.f)
            _freezPoint -= 10.f;
        else if (_healthPoint > 0)
            _healthPoint -= 10.f;
    }
}

void Player::updatePosition(float deltatime)
{
    const sf::ConvexShape shape{ Util::convertBodyToSFMLShape(collider()) };
    const sf::Vector2f playerPos{ Util::pointBy(shape.getLocalBounds(),
                                                { Align::Bottom, Align::Left, Align::Right,
                                                  Align::Top }) };
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(),
                                    { Align::Bottom, Align::Left, Align::Right, Align::Top }));
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

    sf::RectangleShape border(_sprite.getLocalBounds().getSize());
    border.setPosition(_sprite.getPosition());
    border.setOutlineColor(sf::Color::Green);
    border.setOutlineThickness(3);
    border.setFillColor(sf::Color::Transparent);
    border.setOrigin(_sprite.getOrigin());
    border.setScale(_sprite.getScale());
    target.draw(border, states);
}

} // namespace Game
