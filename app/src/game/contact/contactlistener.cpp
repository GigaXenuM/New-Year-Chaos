#include "contactlistener.h"

#include "player/player.h"

namespace Game
{

void ContactListener::BeginContact(b2Contact *contact)
{
    handleContact(contact, true);
}

void ContactListener::EndContact(b2Contact *contact)
{
    handleContact(contact, false);
}

ContactListener *ContactListener::instance()
{
    static auto instance{ std::unique_ptr<ContactListener>{ new ContactListener } };
    return instance.get();
}

UserData ContactListener::toUserData(b2Contact *contact)
{
    UserData data;
    const auto items = {
        reinterpret_cast<AbstractPhysicalItem *>(contact->GetFixtureA()->GetUserData().pointer),
        reinterpret_cast<AbstractPhysicalItem *>(contact->GetFixtureB()->GetUserData().pointer)
    };

    for (auto *item : items)
    {
        assert(item != nullptr && "ContactListener: The user data is not valid.");

        const ItemType type{ item->type() };
        data.itemTypeToItem[type] = item;
        data.types.set(type);
    }

    return data;
}

void ContactListener::handleContact(b2Contact *contact, bool contacted)
{
    const UserData data{ toUserData(contact) };

    if (data.types.test(ItemType::Terrain) && data.types.test(ItemType::Player))
    {
        auto *player{ dynamic_cast<Player *>(data.itemTypeToItem.at(ItemType::Player)) };
        assert(player != nullptr);
        player->updateState(Player::State::OnGround, contacted);
    }
}

} // namespace Game
