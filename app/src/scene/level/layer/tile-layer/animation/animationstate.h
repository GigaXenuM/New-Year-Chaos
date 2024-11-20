#include <tmxlite/Tileset.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

namespace Scene::Level
{

struct AnimationState
{
    sf::Vector2u tileCords;
    sf::Time startTime;
    sf::Time currentTime;
    tmx::Tileset::Tile animTile;
    std::uint8_t flipFlags;
};

} // namespace Scene::Level
