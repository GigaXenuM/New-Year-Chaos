#include "action/actionstorage.h"
#include <box2d/b2_contact.h>

#include "actionvariant.h"
#include "userdata.h"
#include "util/enumflag.h"

#include <box2d/b2_world_callbacks.h>

#pragma once
namespace Game
{

class ContactListener : public b2ContactListener, public Action::ActionStorage<ActionVariant>
{
public:
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;

private:
    UserData toUserData(const b2FixtureUserData &rawData);
    void handleContact(b2Contact *contact, Util::EnumFlag<UserData> data, bool contacted);
};

} // namespace Game
