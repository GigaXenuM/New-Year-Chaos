#include "contactlistener.h"

#include "items/bullet/physicalbullet.h"
#include "player/player.h"
#include <items/deadzone/waterzone.h>
#include <items/loot/tealoot.h>

#include <resources/resourcemanager.h>

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

    if (data.types.test(ItemType::Loot) || data.types.test(ItemType::WaterZone)
        || data.types.test(ItemType::WarmZone) || data.types.test(ItemType::NonCollided)
        || data.types.test(ItemType::WinZone))
        contact->SetEnabled(false);

    if (std::all_of(data.itemTypeToItem.cbegin(), data.itemTypeToItem.cend(),
                    [](const auto &pair) { return pair.first == ItemType::Terrain; }))
        contact->SetEnabled(false);

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

ContactListener::ContactListener()
    : _shootSound{ ResourseManager::getInstance()->getSoundBuffer(SoundType::Snowball) }
{
    _shootSound.setPitch(1.5f);
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

    if (data.types.test(ItemType::WaterZone) && data.types.test(ItemType::Entity) && contacted)
    {
        if (auto *deadWaterZone{
                dynamic_cast<WaterZone *>(data.itemTypeToItem.at(ItemType::WaterZone)) })
        {
            deadWaterZone->playAnimation();
        }
    }
    if (data.types.test(ItemType::DeadZone) && data.types.test(ItemType::Entity) && contacted)
    {
        if (auto *player{ dynamic_cast<Player *>(data.itemTypeToItem.at(ItemType::Entity)) })
            player->kill();
    }
    if (data.types.test(ItemType::WarmZone) && data.types.test(ItemType::Entity))
    {
        if (auto *player{ dynamic_cast<Player *>(data.itemTypeToItem.at(ItemType::Entity)) })
        {
            player->updateState(PhysicalEntity::State::Warming, contacted);

            if (contacted)
                player->setMentadoryHint("Хмм, здається, тут можна зігрітись", true);
        }
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
            const bool nonCollided{ data.itemTypeToItem.contains(ItemType::Loot)
                                    || data.itemTypeToItem.contains(ItemType::NonCollided)
                                    || data.itemTypeToItem.contains(ItemType::WarmZone) };
            if (!shooterCollided && !nonCollided)
            {
                _shootSound.play();
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
