#include "player.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

#include "event/keyevents/keypressevent.h"
#include "event/keyevents/keyreleaseevent.h"

Player::Player(EventHandler *eventHandler) : Graphics::AbstractItem(eventHandler)
{
}

RectF Player::globalRect() const
{
    return RectF{ { 0, 0 }, { 100, 100 } };
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
    {
    }
    if (event->key() == sf::Keyboard::Space)
    {
    }
}

void Player::keyReleaseEvent(KeyReleaseEvent *event)
{
    if (event->key() == sf::Keyboard::Right)
        _movingRight = false;
    if (event->key() == sf::Keyboard::Left)
        _movingLeft = false;
}

void Player::update(float deltatime)
{
    const float speed = 200.f;
    static PointF newPos{};
    if (_movingRight)
       newPos.setX(_position.x() + speed * deltatime);
    if (_movingLeft)
       newPos.setX(_position.x() - speed * deltatime);

    _position = newPos;
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(100.f, 100.f));
    rectangle.setFillColor(sf::Color::Red);

    rectangle.setPosition(_position.x(), _position.y());

    target.draw(rectangle, states);
}
