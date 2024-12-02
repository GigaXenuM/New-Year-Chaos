#include "bot.h"

#include "resources/resourcemanager.h"
#include "util/geometryoperation.h"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <box2d/b2_world.h>

namespace Game
{

namespace
{
b2Body *createCollider(b2World *world, sf::Shape *shape)
{
    b2FixtureDef fixtureDefinition;
    fixtureDefinition.density = 1.0f;
    fixtureDefinition.friction = 0.0f;

    b2BodyDef bodyDefinition;
    bodyDefinition.type = b2_dynamicBody;
    bodyDefinition.fixedRotation = true;

    b2Body *body{ world->CreateBody(&bodyDefinition) };
    Util::createComplexFixture(body, shape, &fixtureDefinition);

    return body;
}
} // namespace

Bot::Bot(b2World *world, sf::Shape *shape)
    : PhysicalEntity(createCollider(world, shape), { 5, 30 }),
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Viking_walk) },
      _pos{ boundingRect().getPosition() }
{
    _sprite.setScale({ _scale, _scale });

    _healthBar.setTexture(
        ResourseManager::getInstance()->getTextures(TextureType::Viking_health_bar)[0]);
    _healthBar.setScale({ _scaleHealthBar, _scaleHealthBar });

    initHealthValueRect();
    updateState(State::Right, true);
}

void Bot::update(float deltatime)
{
    PhysicalEntity::update(deltatime);

    updatePosition(deltatime);
    updateAnimation(deltatime);
    updateHealthValueRectPos();
    updateHealthBarPos(deltatime);
}

void Bot::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
    target.draw(_healthBar, states);
    target.draw(_health, states);
}

void Bot::updateAnimation(float deltatime)
{
    if (isStateActive(State::Right))
        _sprite.setScale({ _scale, _scale });
    else if (isStateActive(State::Left))
        _sprite.setScale({ -_scale, _scale });

    _walkAnimation.start(deltatime, _sprite);
}

void Bot::updateHealthBarPos(float deltatime)
{
    const sf::Vector2f playerPos = _sprite.getPosition();
    const sf::Vector2f healthBarPos
        = { _sprite.getPosition().x,
            _sprite.getPosition().y - _healthBar.getGlobalBounds().height * 4 };
    _healthBar.setOrigin(Util::pointBy(_healthBar.getLocalBounds(),
                                       { Align::Bottom, Align::Left, Align::Right, Align::Top }));

    _healthBar.setPosition(healthBarPos);
}

void Bot::initHealthValueRect()
{
    _health.setFillColor(sf::Color::Yellow);
    _health.setScale({ _scaleHealthBar, _scaleHealthBar });
    _health.setSize(
        { _healthBar.getLocalBounds().width * 0.9f, _healthBar.getLocalBounds().height * 0.35f });
}

void Bot::updateHealthValueRectPos()
{
    _health.setOrigin(Util::pointBy(_health.getLocalBounds(),
                                    { Align::Bottom, Align::Left, Align::Right, Align::Top }));
    const sf::Vector2f yPos
        = { _healthBar.getPosition().x,
            _healthBar.getPosition().y - _health.getGlobalBounds().height * 0.1f };
    _health.setPosition(yPos);
}

void Bot::updatePosition(float deltatime)
{
    static bool isRightEnd{ false };
    static bool isLeftEnd{ false };

    const sf::ConvexShape shape{ Util::convertBodyToSFMLShape(collider()) };

    const sf::Vector2f playerPos{ Util::pointBy(shape.getLocalBounds(), Util::ALIGN_CENTER_STATE) };

    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));

    isLeftEnd = playerPos.x <= _pos.x - _moveLimit;
    isRightEnd = playerPos.x >= _pos.x + _moveLimit;

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
}

} // namespace Game
