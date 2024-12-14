#include "openbridgeactionitem.h"

#include "items/bridge/bridge.h"
#include "items/colliderfactory.h"
#include "player/player.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

OpenBridgeActionItem::OpenBridgeActionItem(b2World *world, sf::Shape *shape, Bridge *bridge)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::NonCollided>(world, { shape }) },
      _bridge{ bridge },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Stone).front() },
      _hint{ "Press E to open the bridge" }
{
    _sprite.scale(1.3f, 1.3f);
}

void OpenBridgeActionItem::showHint()
{
    _needShowHint = true;
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

    _hint.setPosition(Util::pointBy(boundingRect(), { Align::Top }));
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));

    if (_needShowHint)
    {
        _fadeTime += deltatime;
        _hint.update(deltatime);
        if (_fadeTime >= _fadeDuration)
        {
            _fadeTime = 0.f;
            _needShowHint = false;
        }
    }
}

void OpenBridgeActionItem::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);

    if (_needShowHint)
        _hint.draw(target, states);
}

sf::Vector2f OpenBridgeActionItem::position() const
{
    return Util::pointBy(boundingRect(), { Align::Left, Align::Bottom });
}

void OpenBridgeActionItem::execute()
{
    _bridge->collider()->SetType(b2BodyType::b2_dynamicBody);
    _bridge->_type = ItemType::Terrain;
}

} // namespace Game
