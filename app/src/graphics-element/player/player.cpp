#include "player.h"

#include "SFML/Graphics/RenderTarget.hpp"

#include "Resources/ResourceManager.h"
#include "event/keyevents/keypressevent.h"
#include "event/keyevents/keyreleaseevent.h"

Player::Player(EventHandler *eventHandler) : Graphics::AbstractItem(eventHandler)
{
    _sprite.setScale({ 0.2, 0.2 });
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
    if (event->key() == sf::Keyboard::Down)
        _slideMode = true;
    if (event->key() == sf::Keyboard::LShift)
        _runMode = true;
}

void Player::keyReleaseEvent(KeyReleaseEvent *event)
{
    if (event->key() == sf::Keyboard::Right)
        _movingRight = false;
    if (event->key() == sf::Keyboard::Left)
        _movingLeft = false;
    if (event->key() == sf::Keyboard::Down)
        _slideMode = false;
    if (event->key() == sf::Keyboard::LShift)
        _runMode = false;
}

void Player::animation(float deltatime)
{
    _sprite.setOrigin(_sprite.getLocalBounds().width / 2.f, _sprite.getLocalBounds().height / 2.f);

    if (_movingRight || _movingLeft)
    {
        _elapsedTime += deltatime;

        if (_elapsedTime >= _frameTime)
        {
            _elapsedTime = 0.f;

            _currentFrame = (_currentFrame + 1) % _walkTextures.size();
            _sprite.setTexture(_walkTextures[_currentFrame]);

            if (_movingRight)
                _sprite.setScale({ 0.2, 0.2 });
            else if (_movingLeft)
                _sprite.setScale({ -0.2, 0.2 });
        }
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