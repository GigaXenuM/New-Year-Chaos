#pragma once

#include "chunk/chunk.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

namespace Scene::Level
{

class TileLayer final : public sf::Drawable
{
    using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;

public:
    TileLayer(const tmx::Map &map, const tmx::TileLayer &layer);

    ~TileLayer() = default;
    TileLayer(const TileLayer &) = delete;
    TileLayer &operator=(const TileLayer &) = delete;

    void setTile(std::int32_t tileX, std::int32_t tileY, tmx::TileLayer::Tile tile,
                 bool refresh = true);
    tmx::TileLayer::Tile getTile(std::int32_t tileX, std::int32_t tileY);

    void update(sf::Time elapsed);

private:
    void draw(sf::RenderTarget &rendererTarget, sf::RenderStates states) const override;

    Chunk::Ptr &getChunkAndTransform(std::int32_t x, std::int32_t y, sf::Vector2u &chunkRelative);
    void createChunks(const tmx::Map &map, const tmx::TileLayer &layer);
    void updateVisibility(const sf::View &view) const;

    // increasing _chunkSize by 4; fixes render problems when mapsize != chunksize
    // sf::Vector2f _chunkSize = sf::Vector2f(1024.f, 1024.f);
    sf::Vector2f _chunkSize = sf::Vector2f(512.f, 512.f);
    sf::Vector2u _chunkCount;
    sf::Vector2u _mapTileSize; // general Tilesize of Map

    TextureResource _textureResource;

    std::vector<Chunk::Ptr> _chunks;
    mutable std::vector<Chunk *> _visibleChunks;
};

} // namespace Scene::Level
