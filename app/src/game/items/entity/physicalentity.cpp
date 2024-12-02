#include "physicalentity.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>

namespace Game
{

PhysicalEntity::PhysicalEntity(b2Body *collider, const Context &context)
    : AbstractPhysicalItem{ collider }, _context{ context }
{
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
    // sf::RectangleShape bulletShape{ { 10, 10 } };
    // bulletShape.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE)
    //                         + sf::Vector2f{ boundingRect().getSize().x, 0.f }
    //                         + sf::Vector2f{ 20.f, 0.f });

    // b2FixtureDef fixtureDefinition;
    // fixtureDefinition.density = 1.0f;
    // fixtureDefinition.friction = 0.0f;

    // b2BodyDef bodyDefinition;
    // bodyDefinition.type = b2_dynamicBody;
    // bodyDefinition.fixedRotation = true;
    // // bodyDefinition.userData.pointer = static_cast<uintptr_t>(UserData::Bullet);

    // b2Body *bulletBody{ collider()->GetWorld()->CreateBody(&bodyDefinition) };
    // Util::createComplexFixture(bulletBody, &bulletShape, &fixtureDefinition);

    // _bullets.push_back(bulletBody);
}

void PhysicalEntity::update(float deltatime)
{
    AbstractPhysicalItem::update(deltatime);

    b2Vec2 velocity{ collider()->GetLinearVelocity() };

    velocity.x = 0.0f;
    if (isStateActive(State::Left))
        velocity.x -= _context.velocity;
    if (isStateActive(State::Right))
        velocity.x += _context.velocity;

    const bool onGround{ isStateActive(State::OnGround) };
    const bool needJumping{ isStateActive(State::Jump) && onGround };

    if (needJumping)
        velocity.y = 0.0f;

    if (velocity.x != 0.0f && velocity.y != 0.0f && onGround)
        velocity *= _context.velocity / velocity.Length();
    collider()->SetLinearVelocity(velocity);

    if (needJumping)
    {
        b2Vec2 jumpImpulse(0.0f, -collider()->GetMass() * _context.jumpImpulse);
        collider()->ApplyLinearImpulseToCenter(jumpImpulse, true);
    }
}

} // namespace Game
