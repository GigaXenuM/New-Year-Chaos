#include "background.h"

#include "resources/resourcemanager.h"
#include "util/geometryoperation.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

namespace
{
const sf::Vector2f BACKGROUND_SCALE{ 2.f, 2.f };

sf::FloatRect backgroundRect(const sf::View *view, const sf::Vector2f &offset)
{
    const sf::Vector2f size{ view->getSize() };
    const sf::Vector2f center{ view->getCenter() };
    const sf::Vector2f position{ (center.x - (size.x / 2)), center.y - (size.y / 2) };
    return { position + offset, size };
}
} // namespace

Background::Background(const sf::View *view)
    : _view{ view },
      _startViewPos{ backgroundRect(_view, {}).getPosition() },
      _layers{
          ParallaxLayer{ ResourseManager::getInstance()->getTextures(TextureType::Background).at(0),
                         0 },
          ParallaxLayer{ ResourseManager::getInstance()->getTextures(TextureType::Background).at(1),
                         0 },
          ParallaxLayer{ ResourseManager::getInstance()->getTextures(TextureType::Background).at(2),
                         20 },
          ParallaxLayer{ ResourseManager::getInstance()->getTextures(TextureType::Background).at(3),
                         -30 }
      }
{
}

void Background::update(float deltatime)
{
    for (auto &layer : _layers)
        layer.offset.x -= layer.speed * deltatime;

    updateSprites();
}

void Background::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto &layer : _layers)
        target.draw(layer.sprite, states);
}

void Background::updateSprites()
{
    const auto viewRect{ backgroundRect(_view, {}) };
    const sf::Vector2f viewDeltaPos{ _startViewPos - viewRect.getPosition() };

    for (auto &layer : _layers)
    {
        const sf::Vector2f offset{ layer.offset + viewDeltaPos };
        const auto rect{ backgroundRect(_view, offset) };

        const sf::Vector2f viewSize{ rect.getSize() };
        const sf::Vector2f textureSize{ layer.sprite.getTexture()->getSize() };
        const sf::Vector2f spriteScaling{ viewSize.x / textureSize.x, viewSize.y / textureSize.y };
        layer.sprite.setScale(spriteScaling);

        layer.sprite.setPosition(viewRect.getPosition());
        sf::IntRect textureRect{ sf::Vector2i{ rect.getPosition() },
                                 sf::Vector2i{ sf::Vector2f{ rect.width / spriteScaling.x,
                                                             rect.height / spriteScaling.y } } };
        layer.sprite.setTextureRect(textureRect);
    }
}

} // namespace Game
