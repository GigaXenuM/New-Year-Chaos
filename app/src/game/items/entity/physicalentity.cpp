#include "physicalentity.h"

#include "weapon/snowballgun.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>

namespace Game
{

PhysicalEntity::PhysicalEntity(b2Body *collider, const Context &context,
                               std::unique_ptr<IWeapon> weapon)
    : AbstractPhysicalItem{ collider }, _context{ context }, _weapon{ std::move(weapon) }
{
}

void PhysicalEntity::updatePhysics()
{
    if (isStateActive(State::Dead))
        return;

    b2Vec2 velocity{ collider()->GetLinearVelocity() };

    velocity.x = 0.0f;

    float runVelocity = _context.velocity;

    if (isStateActive(State::Run))
        runVelocity *= 2;

    if (isStateActive(State::Left))
        velocity.x -= runVelocity;
    if (isStateActive(State::Right))
        velocity.x += runVelocity;

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
    _weapon->shoot(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE), target);
}

void PhysicalEntity::update(float deltatime)
{
    AbstractPhysicalItem::update(deltatime);

    _weapon->update(deltatime);
}

IWeapon *const PhysicalEntity::weapon() const
{
    return _weapon.get();
}

} // namespace Game
