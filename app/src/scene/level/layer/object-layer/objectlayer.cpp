#include "objectlayer.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <tmxlite/ObjectGroup.hpp>

#include <iostream>

namespace
{

class PolygonShape : public sf::Shape
{
public:
    PolygonShape(const tmx::Object &tmxObject) : _tmxObject{ tmxObject }
    {
    }

    std::size_t getPointCount() const override
    {
        return _tmxObject.getPoints().size();
    }
    sf::Vector2f getPoint(std::size_t index) const override
    {
        tmx::Vector2f point{ _tmxObject.getPoints().at(index) };
        return { point.x, point.y };
    }

private:
    tmx::Object _tmxObject;
};

} // namespace

namespace Scene::Level
{

ObjectLayer::ObjectLayer(const tmx::ObjectGroup &layer)
{
    fillObjects(layer);
}

std::vector<sf::Shape *> ObjectLayer::getObjects(const std::string &objectName) const
{
    const std::vector<std::unique_ptr<sf::Shape>> &objects{ _collidersMap.at(objectName) };

    std::vector<sf::Shape *> result;
    result.reserve(objects.size());
    std::transform(objects.cbegin(), objects.cend(), std::back_inserter(result),
                   [](const std::unique_ptr<sf::Shape> &obj) { return obj.get(); });

    return result;
}

ObjectLayer::~ObjectLayer() = default;

void ObjectLayer::draw(sf::RenderTarget &rendererTarget, sf::RenderStates states) const
{
    for (const auto &[_, colliders] : _collidersMap)
        for (const auto &collider : colliders)
            rendererTarget.draw(*collider, states);
}

void ObjectLayer::fillObjects(const tmx::ObjectGroup &layer)
{
    const std::vector<tmx::Object> &tmxObjects{ layer.getObjects() };
    for (const tmx::Object &tmxObject : tmxObjects)
    {
        std::unique_ptr<sf::Shape> object;
        switch (tmxObject.getShape())
        {
        case tmx::Object::Shape::Rectangle:
            object = std::make_unique<sf::RectangleShape>(
                sf::Vector2f{ tmxObject.getAABB().width, tmxObject.getAABB().height });
            break;
        case tmx::Object::Shape::Polygon:
            object = std::make_unique<PolygonShape>(tmxObject);
            break;
        default:
            break;
        }
        object->setPosition(tmxObject.getPosition().x, tmxObject.getPosition().y);
        object->setFillColor(sf::Color::Transparent);
        object->setOutlineColor(sf::Color::Red);
        object->setOutlineThickness(1);

        if (std::any_of(_collidersMap.cbegin(), _collidersMap.cend(),
                        [name = tmxObject.getName()](const auto &nameToCollider)
                        { return nameToCollider.first == name; }))
        {
            std::cerr << "The collader with the name \"" << tmxObject.getName()
                      << "\" already exists." << std::endl;
            assert(false);
        }
        _collidersMap[tmxObject.getName()].push_back(std::move(object));
    }
}

} // namespace Scene::Level