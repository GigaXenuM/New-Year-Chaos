#include "bot3.h"

#include "items/loot/keyloot.h"
#include "player.h"

#include "items/bullet/physicalbullet.h"
#include "items/colliderfactory.h"
#include "weapon/snowballgun.h"

#include "resources/resourcemanager.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

Bot3::Bot3(b2World *world, sf::Shape *shape, const PhysicalEntity *targetEntity)
    : Enemy(world, shape, targetEntity, 1.f, 25.f),
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Snowman_3_walk) },
      _deadAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Snowman_3_dead) },
      _hurtAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Snowman_3_hurt) },
      _throwAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Snowman_3_throw) }
{
    setupSprites();
}

void Bot3::update(float deltatime)
{
    Enemy::update(deltatime);

    if (isStateActive(State::Dead) && _deadAnimation.isFinished())
        updateState(State::RemoveMe, true);

    updateAnimation(deltatime);
}

void Bot3::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const std::unique_ptr<PhysicalBullet> &bullet : weapon()->bullets())
        target.draw(*bullet, states);

    target.draw(_sprite, states);
    target.draw(_healthBar, states);
    target.draw(_health, states);
}

std::vector<AbstractPhysicalItem *> Bot3::dropLoots()
{
    sf::RectangleShape shape{ { 50, 50 } };
    shape.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));
    return std::vector<AbstractPhysicalItem *>{ new KeyLoot(collider()->GetWorld(), &shape,
                                                            ActionVariant::PickUpKeyBridge) };
}

void Bot3::setupSprites()
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

void Bot3::updateAnimation(float deltatime)
{
    if (isStateActive(State::Dead))
    {
        _deadAnimation.start(deltatime, _sprite, false);
        return;
    }
    if (isStateActive(State::Right) || isStateActive(State::Left))
    {
        if (isStateActive(State::Right))
            _sprite.setScale({ _scale, _scale });
        else if (isStateActive(State::Left))
            _sprite.setScale({ -_scale, _scale });
        _walkAnimation.start(deltatime, _sprite);
    }
}

} // namespace Game
