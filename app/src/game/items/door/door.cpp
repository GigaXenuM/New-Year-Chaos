#include "door.h"

#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Game
{

namespace
{
constexpr const char *const OPEN_DOOR_HINT{ "Press E to open the door" };
constexpr const char *const CLOSE_DOOR_HINT{ "Press E to close the door" };
} // namespace

Door::Door(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::TerrainObstacle>(world, { shape }) },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Door).front() },
      _doorSideSprite{ ResourseManager::getInstance()->getTextures(TextureType::DoorSide).front() },
      _hintText{ OPEN_DOOR_HINT },
      _doorScaling{ 0.2, 0.2, _sprite.getScale().x }
{
    _sprite.scale(_doorScaling.get(), 1.2);
    _doorSideSprite.scale(1, 1.2);
}

std::string Door::hintText() const
{
    return _hintText;
}

bool Door::needDestroying() const
{
    return false;
}

void Door::update(float deltatime)
{
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), { Align::Left }));
    _sprite.setPosition(Util::pointBy(boundingRect(), { Align::Left }));
    _doorSideSprite.setOrigin(Util::pointBy(_doorSideSprite.getLocalBounds(), { Align::Left }));
    _doorSideSprite.setPosition(Util::pointBy(boundingRect(), { Align::Left }));

    float openDoorSpeed{ deltatime * 2 };

    const bool isDoorOpened{ _type == ItemType::NonCollided };
    if (isDoorOpened && !_doorScaling.isMax())
        _doorScaling.move(openDoorSpeed);
    else if (!isDoorOpened && !_doorScaling.isMin())
        _doorScaling.move(-openDoorSpeed);

    const float yScaleDoor{ _sprite.getScale().y };
    const float yScaleDoorSide{ _doorSideSprite.getScale().y };
    _sprite.setScale({ _doorScaling.get(), yScaleDoor });
    _doorSideSprite.setScale({ _doorScaling.max() - _doorScaling.get(), yScaleDoorSide });
}

void Door::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
    target.draw(_doorSideSprite, states);
}

sf::Vector2f Door::position() const
{
    return Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE);
}

void Door::execute()
{
    const bool isDoorOpened{ _type == ItemType::NonCollided };
    _type = isDoorOpened ? ItemType::TerrainObstacle : ItemType::NonCollided;
    _hintText = isDoorOpened ? OPEN_DOOR_HINT : CLOSE_DOOR_HINT;
}

} // namespace Game
