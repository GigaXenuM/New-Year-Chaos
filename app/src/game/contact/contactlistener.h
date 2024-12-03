#include "items/abstractphysicalitem.h"
#include "items/itemtype.h"
#include "util/enumflag.h"

#include <box2d/b2_contact.h>
#include <box2d/b2_world_callbacks.h>

#include <unordered_map>

#pragma once
namespace Game
{

struct UserData
{
    std::unordered_map<Game::ItemType, AbstractPhysicalItem *> itemTypeToItem;
    Util::EnumFlag<ItemType> types;
};

class ContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;

    static ContactListener *instance();

private:
    ContactListener() = default;

    UserData toUserData(b2Contact *contact);
    void handleContact(b2Contact *contact, bool contacted);
};

} // namespace Game
