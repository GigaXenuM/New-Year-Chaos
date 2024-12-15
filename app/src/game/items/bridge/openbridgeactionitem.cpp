#include "openbridgeactionitem.h"

#include "items/bridge/bridge.h"
#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

OpenBridgeActionItem::OpenBridgeActionItem(b2World *world, sf::Shape *shape, Bridge *bridge)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::NonCollided>(world, { shape }) },
      _bridge{ bridge },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Stone).front() },
      _hintText{ "Press E to open the bridge" }
{
    _sprite.scale(1.3f, 1.3f);
}

std::string OpenBridgeActionItem::hintText() const
{
    return _hintText;
}

bool OpenBridgeActionItem::needDestroying() const
{
    return false;
}

void OpenBridgeActionItem::update(float deltatime)
{

    const sf::Vector2f position{ _bridge->_actionPos };
    collider()->SetTransform({ position.x, position.y - boundingRect().height },
                             collider()->GetTransform().q.GetAngle());

    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));
}

void OpenBridgeActionItem::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

sf::Vector2f OpenBridgeActionItem::position() const
{
    return Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE);
}

void OpenBridgeActionItem::execute()
{
    _bridge->collider()->SetType(b2BodyType::b2_dynamicBody);
    _bridge->_type = ItemType::Terrain;
    _hintText.clear();
}

} // namespace Game
