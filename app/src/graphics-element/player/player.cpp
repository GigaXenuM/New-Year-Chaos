#include "player.h"

#include "SFML/Graphics/RenderTarget.hpp"

#include "Resources/ResourceManager.h"
#include "event/keyevents/keypressevent.h"
#include "event/keyevents/keyreleaseevent.h"

Player::Player(EventHandler *eventHandler)
    : Graphics::AbstractItem(eventHandler),
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_walk) },
      _runAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_run) },
      _jumpAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_jump) }
{
    _walkAnimation.start();
    _sprite.setScale(_scaleFactors);
}

RectF Player::globalRect() const
{
    return {};
}

RectF Player::localRect() const
{
    return {};
}

PointF Player::center() const
{
    return {};
}

void Player::setPos(PointF position)
{
}

void Player::setOrigin(Align origin)
{
}

void Player::keyPressEvent(KeyPressEvent *event)
{
    if (event->key() == sf::Keyboard::Right)
        _movingRight = true;
    if (event->key() == sf::Keyboard::Left)
        _movingLeft = true;
}

void Player::keyReleaseEvent(KeyReleaseEvent *event)
{
    if (event->key() == sf::Keyboard::Right)
        _movingRight = false;
    if (event->key() == sf::Keyboard::Left)
        _movingLeft = false;
}

void Player::animation(float deltatime)
{
    _sprite.setOrigin(_sprite.getLocalBounds().width / 2.f, _sprite.getLocalBounds().height / 2.f);
    if (_movingRight || _movingLeft)
    {
        _walkAnimation.update(deltatime, _sprite);
        if (_movingRight)
            _sprite.setScale(_scaleFactors);
        else if (_movingLeft)
            _sprite.setScale(_rscaleFactors);
    }
    else
    {
        _sprite.setTexture(_walkTextures[0]);
    }
}

void Player::updatePosition(float deltatime)
{
    if (_movingRight)
        _position.setX(_position.x() + _speed * deltatime);
    if (_movingLeft)
        _position.setX(_position.x() - _speed * deltatime);
    _sprite.setPosition(_position.x(), _position.y());
}

void Player::update(float deltatime)
{
    updatePosition(deltatime);
    animation(deltatime);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}