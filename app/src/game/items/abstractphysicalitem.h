#pragma once

#include "item/drawable.h"

#include "items/itemtype.h"
#include "util/geometryoperation.h"

#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>

#include <cstdint>

struct b2Body;

namespace Game
{

class AbstractPhysicalItem : public Graphics::Drawable
{
public:
    explicit AbstractPhysicalItem(b2Body *collider);
    virtual ~AbstractPhysicalItem();

    sf::FloatRect boundingRect() const;

    virtual ItemType type() const = 0;
    virtual void updatePhysics(){};

    virtual bool needDestroying() const = 0;
    void destroyCollider();

    b2Body *collider() const;

private:
    b2Body *_collider{ nullptr };
};

inline AbstractPhysicalItem::AbstractPhysicalItem(b2Body *collider) : _collider{ collider }
{
    _collider->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    for (b2Fixture *fixture{ _collider->GetFixtureList() }; fixture != nullptr;
         fixture = fixture->GetNext())
    {
        fixture->GetUserData().pointer = _collider->GetUserData().pointer;
    }
}

inline AbstractPhysicalItem::~AbstractPhysicalItem()
{
}

inline sf::FloatRect AbstractPhysicalItem::boundingRect() const
{
    return Util::convertBodyToSFMLShape(_collider).getGlobalBounds();
}

inline b2Body *AbstractPhysicalItem::collider() const
{
    return _collider;
}

inline void AbstractPhysicalItem::destroyCollider()
{
    b2World *world{ _collider->GetWorld() };
    world->DestroyBody(_collider);
}

} // namespace Game
