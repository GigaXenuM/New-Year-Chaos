#pragma once

#include <SFML/Graphics/Drawable.hpp>

#include <memory>
#include <unordered_map>

#include <string>

namespace tmx
{
class ObjectGroup;
} // namespace tmx

namespace sf
{
class Shape;
class RectangleShape;
} // namespace sf

namespace Game::Level
{

class ObjectLayer final
{
    using CollidersMap = std::unordered_map<std::string, std::vector<std::unique_ptr<sf::Shape>>>;

public:
    ObjectLayer(const tmx::ObjectGroup &layer);
    ~ObjectLayer();

    std::vector<sf::Shape *> objects(const std::string &objectName) const;
    std::vector<sf::Shape *> objects() const;

private:
    void fillObjects(const tmx::ObjectGroup &layer);

    CollidersMap _collidersMap;
};

} // namespace Game::Level
