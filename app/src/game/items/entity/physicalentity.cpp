#include "physicalentity.h"

#include "items/bullet/snowball.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>

namespace Game
{

PhysicalEntity::PhysicalEntity(b2Body *collider, const Context &context)
    : AbstractPhysicalItem{ collider }, _context{ context }
{
}

void PhysicalEntity::updatePhysics()
{
    b2Vec2 velocity{ collider()->GetLinearVelocity() };

    velocity.x = 0.0f;

    float runVelocity = _context.velocity;

    if (isStateActive(State::Run))
        runVelocity *= 2;

    if (isStateActive(State::Left))
        velocity.x -= runVelocity;
    if (isStateActive(State::Right))
        velocity.x +=runVelocity;

    const bool onGround{ isStateActive(State::OnGround) };
    const bool needJumping{ isStateActive(State::Jump) && onGround };

    if (needJumping)
        velocity.y = 0.0f;

    if (velocity.x != 0.0f && velocity.y != 0.0f && onGround)
        velocity.x *= runVelocity / velocity.Length();

    collider()->SetLinearVelocity(velocity);

    if (needJumping)
    {
        b2Vec2 jumpImpulse(0.0f, -collider()->GetMass() * _context.jumpImpulse);
        collider()->ApplyLinearImpulseToCenter(jumpImpulse, true);

        if (isStateActive(State::PrepareGroundDetach))
        {
            updateState(State::PrepareGroundDetach, false);
            updateState(State::OnGround, false);
        }
    }

    for (std::unique_ptr<PhysicalBullet> &bullet : _bullets)
        bullet->updatePhysics();
}

void PhysicalEntity::updateState(State state, bool isActive)
{
    isActive ? _state.set(state) : _state.unset(state);
}

bool PhysicalEntity::isStateActive(State state) const
{
    return _state.test(state);
}

PhysicalEntity::~PhysicalEntity() = default;

void PhysicalEntity::shoot(const sf::Vector2f &target)
{
    sf::RectangleShape bulletShape{ { 10, 10 } };
    bulletShape.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));

    auto *bullet{ new SnowBall{ collider()->GetWorld(), &bulletShape, this,
                                SnowBall::Context{ 50.f, target } } };
    bullet->impulse();
    _bullets.push_back(std::unique_ptr<PhysicalBullet>(bullet));
}

void PhysicalEntity::update(float deltatime)
{
    AbstractPhysicalItem::update(deltatime);

    for (std::unique_ptr<PhysicalBullet> &bullet : _bullets)
    {
        bullet->update(deltatime);
        if (bullet->isStateActive(PhysicalBullet::State::Collide))
        {
            bullet->destroyCollider();
            bullet.reset();
        }
    }
    _bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(),
                                  [](auto &bullet) { return bullet == nullptr; }),
                   _bullets.end());
}

} // namespace Game
