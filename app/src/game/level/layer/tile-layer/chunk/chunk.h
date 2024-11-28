#pragma once

#include "chunkarray.h"
#include "level/layer/tile-layer/animation/animationstate.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/detail/Log.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Game::Level
{

class Chunk final : public sf::Transformable, public sf::Drawable
{
    using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;

public:
    using Ptr = std::unique_ptr<Chunk>;

    Chunk(const tmx::TileLayer &layer, std::vector<const tmx::Tileset *> tilesets,
          const sf::Vector2f &position, const sf::Vector2f &tileCount, const sf::Vector2u &tileSize,
          std::size_t rowSize, TextureResource &tr,
          const std::map<std::uint32_t, tmx::Tileset::Tile> &animTiles);

    void generateTiles(bool registerAnimation = false);
    ~Chunk() = default;
    Chunk(const Chunk &) = delete;
    Chunk &operator=(const Chunk &) = delete;

    std::vector<AnimationState> &getActiveAnimations();

    tmx::TileLayer::Tile getTile(std::int32_t x, std::int32_t y) const;

    void setTile(std::int32_t x, std::int32_t y, tmx::TileLayer::Tile tile, bool refresh);

    void maybeRegenerate(bool refresh);

    std::int32_t calcIndexFrom(std::int32_t x, std::int32_t y) const;

    bool empty() const;

    void flipY(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3,
               sf::Vector2f *v4, sf::Vector2f *v5);

    void flipX(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3,
               sf::Vector2f *v4, sf::Vector2f *v5);

    void flipD(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3,
               sf::Vector2f *v4, sf::Vector2f *v5);

    void doFlips(std::uint8_t bits, sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2,
                 sf::Vector2f *v3, sf::Vector2f *v4, sf::Vector2f *v5);

private:
    void draw(sf::RenderTarget &rt, sf::RenderStates states) const override;

    sf::Vector2u _mapTileSize;                       // general Tilesize of Map
    sf::Vector2f _chunkTileCount;                    // chunk tilecount
    std::vector<tmx::TileLayer::Tile> _chunkTileIDs; // stores all tiles in this chunk for
                                                     // later manipulation
    std::vector<sf::Color> _chunkColors;             // stores colors for extended color effects
    std::map<std::uint32_t, tmx::Tileset::Tile> _animTiles; // animation catalogue
    std::vector<AnimationState> _activeAnimations;          // Animations to be done in this chunk
    std::vector<ChunkArray::Ptr> _chunkArrays;
};

} // namespace Game::Level
