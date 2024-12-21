#pragma once

#include "items/abstractphysicalitem.h"
#include "items/itemtype.h"
#include "util/enumflag.h"

#include <SFML/Audio/Sound.hpp>

#include <box2d/b2_contact.h>
#include <box2d/b2_world_callbacks.h>

#include <unordered_map>

struct b2Fixture;

namespace Game
{

struct UserData
{
    std::unordered_map<Game::ItemType, AbstractPhysicalItem *> itemTypeToItem;
    std::unordered_map<Game::ItemType, b2Fixture *> itemTypeToFixture;
    Util::EnumFlag<ItemType> types;
};

class ContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;

    static ContactListener *instance();

private:
    ContactListener();

    UserData toUserData(b2Contact *contact);
    void handleContact(b2Contact *contact, bool contacted);

    sf::Sound _shootSound;
    sf::Sound _lootSound;
};

} // namespace Game
