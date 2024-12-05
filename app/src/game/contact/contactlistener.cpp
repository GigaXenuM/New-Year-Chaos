#include "contactlistener.h"

#include "items/bullet/physicalbullet.h"
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

void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    const UserData data{ toUserData(contact) };

    if (data.types.test(ItemType::Bullet))
    {
        auto *snowBall{ dynamic_cast<PhysicalBullet *>(data.itemTypeToItem.at(ItemType::Bullet)) };
        assert(snowBall != nullptr);
        if (contact)
        {
            const bool shooterCollided{
                std::any_of(data.itemTypeToItem.cbegin(), data.itemTypeToItem.cend(),
                            [snowBall](auto &value) { return snowBall->shooter() == value.second; })
            };
            if (shooterCollided)
                contact->SetEnabled(false);
        }
    }
}

ContactListener *ContactListener::instance()
{
    static auto instance{ std::unique_ptr<ContactListener>{ new ContactListener } };
    return instance.get();
}

UserData ContactListener::toUserData(b2Contact *contact)
{
    UserData data;
    const auto items = { std::pair{ reinterpret_cast<AbstractPhysicalItem *>(
                                        contact->GetFixtureA()->GetUserData().pointer),
                                    contact->GetFixtureA() },
                         std::pair{ reinterpret_cast<AbstractPhysicalItem *>(
                                        contact->GetFixtureB()->GetUserData().pointer),
                                    contact->GetFixtureB() } };

    for (auto &[item, fixture] : items)
    {
        assert(item != nullptr && "ContactListener: The user data is not valid.");

        const ItemType type{ item->type() };
        data.itemTypeToItem[type] = item;
        data.itemTypeToFixture[type] = fixture;
        data.types.set(type);
    }

    return data;
}

void ContactListener::handleContact(b2Contact *contact, bool contacted)
{
    const UserData data{ toUserData(contact) };

    if (data.types.test(ItemType::Terrain) && data.types.test(ItemType::Entity))
    {
        auto *entity{ dynamic_cast<PhysicalEntity *>(data.itemTypeToItem.at(ItemType::Entity)) };
        assert(entity != nullptr);
        entity->updateState(contacted ? PhysicalEntity::State::OnGround
                                      : PhysicalEntity::State::PrepareGroundDetach,
                            true);
    }

    if (data.types.test(ItemType::Bullet) && contacted)
    {
        auto *bullet{ dynamic_cast<PhysicalBullet *>(data.itemTypeToItem.at(ItemType::Bullet)) };
        assert(bullet != nullptr);
        if (contact)
        {
            const bool shooterCollided{
                std::any_of(data.itemTypeToItem.cbegin(), data.itemTypeToItem.cend(),
                            [bullet](auto &value) { return bullet->shooter() == value.second; })
            };
            if (!shooterCollided)
            {
                bullet->updateState(PhysicalBullet::State::Collide, true);

                if (data.types.test(ItemType::Entity))
                {
                    auto *entity{ dynamic_cast<PhysicalEntity *>(
                        data.itemTypeToItem.at(ItemType::Entity)) };
                    assert(entity != nullptr);
                    entity->damage(bullet->power());
                }
            }
        }
    }
}

} // namespace Game
