#pragma once

#include "item/drawable.h"

#include "items/itemtype.h"
#include "util/geometryoperation.h"

#include <box2d/b2_fixture.h>

#include <cstdint>

struct b2Body;

namespace Game
{

class AbstractPhysicalItem : public Graphics::Drawable
{
public:
    explicit AbstractPhysicalItem(b2Body *collider);
    sf::FloatRect boundingRect() const;

    virtual ItemType type() const = 0;

protected:
    b2Body *collider();

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

inline sf::FloatRect AbstractPhysicalItem::boundingRect() const
{
    return Util::convertBodyToSFMLShape(_collider).getGlobalBounds();
}

inline b2Body *AbstractPhysicalItem::collider()
{
    return _collider;
}

} // namespace Game
