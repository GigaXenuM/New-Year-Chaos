#include "contactlistener.h"

#include <iostream>

namespace Game
{

void ContactListener::BeginContact(b2Contact *contact)
{
    handleContact(contact,
                  { toUserData(contact->GetFixtureA()->GetUserData()),
                    toUserData(contact->GetFixtureB()->GetUserData()) },
                  true);
}

void ContactListener::EndContact(b2Contact *contact)
{
    handleContact(contact,
                  { toUserData(contact->GetFixtureA()->GetUserData()),
                    toUserData(contact->GetFixtureB()->GetUserData()) },
                  false);
}

UserData ContactListener::toUserData(const b2FixtureUserData &rawData)
{
    return static_cast<UserData>(rawData.pointer);
}

void ContactListener::handleContact(b2Contact *contact, Util::EnumFlag<UserData> data,
                                    bool contacted)
{
    if (data.test(UserData::Terrain) && data.test(UserData::Player))
    {
        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);
        const b2Vec2 normal{ worldManifold.normal };

        if (std::fabs(normal.x) > 0.99f && std::fabs(normal.y) < 0.6f)
        {
            executeActions(ActionVariant::PlayerAboveGround);
            return;
        }

        executeActions(contacted ? ActionVariant::PlayerOnGround
                                 : ActionVariant::PlayerAboveGround);
    }
}

} // namespace Game
