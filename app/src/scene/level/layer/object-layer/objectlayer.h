#pragma once

#include "objectvariant.h"

#include <SFML/Graphics/Drawable.hpp>

#include <memory>
#include <unordered_map>

namespace tmx
{
class ObjectGroup;
} // namespace tmx

namespace sf
{
class Shape;
class RectangleShape;
} // namespace sf

namespace Scene::Level
{

class ObjectLayer final : public sf::Drawable
{
    using CollidersMap = std::unordered_map<std::string, std::vector<std::unique_ptr<sf::Shape>>>;

public:
    ObjectLayer(const tmx::ObjectGroup &layer);
    ~ObjectLayer();

    std::vector<sf::Shape *> getObjects(const std::string &objectName) const;

private:
    void draw(sf::RenderTarget &rendererTarget, sf::RenderStates states) const override;

    void fillObjects(const tmx::ObjectGroup &layer);

    CollidersMap _collidersMap;
};

} // namespace Scene::Level
