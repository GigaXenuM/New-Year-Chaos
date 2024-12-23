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

bool PhysicalEntity::needDestroying() const
{
    return isStateActive(State::RemoveMe);
}

void PhysicalEntity::updatePhysics()
{
    if (isStateActive(State::Dead) && isStateActive(State::OnGround))
    {
        collider()->SetType(b2BodyType::b2_staticBody);
        _type = ItemType::NonCollided;
        return;
    }

    collider()->SetType(b2BodyType::b2_dynamicBody);
    _type = ItemType::Entity;

    b2Vec2 velocity{ collider()->GetLinearVelocity() };

    const bool onGround{ isStateActive(State::OnGround) };
    const bool needJumping{ isStateActive(State::Jump) && onGround };
    velocity.x = 0.0f;

    if (!isStateActive(State::Dead))
    {
        float runVelocity = _context.velocity;

        if (isStateActive(State::Run) && _stamina > 0.f)
            runVelocity *= 2;

        if (isStateActive(State::Left))
            velocity.x -= runVelocity;
        if (isStateActive(State::Right))
            velocity.x += runVelocity;

        if (needJumping)
            velocity.y = 0.0f;

        if (velocity.x != 0.0f && velocity.y != 0.0f && onGround)
            velocity.x *= runVelocity / velocity.Length();
    }

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
    if (isStateActive(State::Dead))
        return;

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
