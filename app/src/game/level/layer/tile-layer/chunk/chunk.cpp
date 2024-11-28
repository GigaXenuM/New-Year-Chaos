#include "chunk.h"

namespace Game::Level
{

Chunk::Chunk(const tmx::TileLayer &layer, std::vector<const tmx::Tileset *> tilesets,
             const sf::Vector2f &position, const sf::Vector2f &tileCount,
             const sf::Vector2u &tileSize, size_t rowSize, TextureResource &tr,
             const std::map<uint32_t, tmx::Tileset::Tile> &animTiles)
    : _animTiles(animTiles), _chunkTileCount{ tileCount.x, tileCount.y }, _mapTileSize{ tileSize }
{
    setPosition(position);

    const auto &tiles = layer.getTiles();

    // Process tilesets and create arrays for later manipulation
    for (const auto &tileset : tilesets)
    {
        const auto &imagePath = tileset->getImagePath();
        if (imagePath.empty())
        {
            tmx::Logger::log("This example does not support Collection of Images tilesets",
                             tmx::Logger::Type::Info);
            tmx::Logger::log("Chunks using " + tileset->getName() + " will not be created",
                             tmx::Logger::Type::Info);
            continue;
        }

        _chunkArrays.emplace_back(std::make_unique<ChunkArray>(*(tr.at(imagePath)), *tileset));
    }

    // Calculate starting positions
    const std::size_t xStart = static_cast<std::size_t>(position.x / tileSize.x);
    const std::size_t yStart = static_cast<std::size_t>(position.y / tileSize.y);

    // Reserve space to reduce reallocations
    _chunkTileIDs.reserve(tileCount.y * tileCount.x);

    for (std::size_t y = yStart; y < yStart + tileCount.y; ++y)
    {
        for (std::size_t x = xStart; x < xStart + tileCount.x; ++x)
        {
            const std::size_t idx = y * rowSize + x;
            if (idx < tiles.size()) // Add bounds checking for safety
            {
                _chunkTileIDs.emplace_back(tiles[idx]);
            }
            else
            {
                tmx::Logger::log("Index out of bounds while accessing tiles",
                                 tmx::Logger::Type::Warning);
            }
        }
    }
    generateTiles(true);
}

std::vector<AnimationState> &Chunk::getActiveAnimations()
{
    return _activeAnimations;
}

tmx::TileLayer::Tile Chunk::getTile(int32_t x, int32_t y) const
{
    return _chunkTileIDs[calcIndexFrom(x, y)];
}

void Chunk::setTile(int32_t x, int32_t y, tmx::TileLayer::Tile tile, bool refresh)
{
    _chunkTileIDs[calcIndexFrom(x, y)] = tile;
    maybeRegenerate(refresh);
}

void Chunk::maybeRegenerate(bool refresh)
{
    if (refresh)
    {
        for (const auto &ca : _chunkArrays)
        {
            ca->reset();
        }
        generateTiles();
    }
}

int32_t Chunk::calcIndexFrom(int32_t x, int32_t y) const
{
    return x + y * static_cast<std::int32_t>(_chunkTileCount.x);
}

bool Chunk::empty() const
{
    return _chunkArrays.empty();
}

void Chunk::flipY(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3,
                  sf::Vector2f *v4, sf::Vector2f *v5)
{
    // Flip Y
    sf::Vector2f tmp0 = *v0;
    v0->y = v5->y;
    v3->y = v5->y;
    v5->y = tmp0.y;
    sf::Vector2f tmp2 = *v2;
    v2->y = v1->y;
    v4->y = v1->y;
    v1->y = tmp2.y;
}

void Chunk::flipX(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3,
                  sf::Vector2f *v4, sf::Vector2f *v5)
{
    // Flip X
    sf::Vector2f tmp0 = *v0;
    v0->x = v1->x;
    v3->x = v1->x;
    v1->x = tmp0.x;
    sf::Vector2f tmp2 = *v2;
    v2->x = v5->x;
    v4->x = v5->x;
    v5->x = tmp2.x;
}

void Chunk::flipD(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3,
                  sf::Vector2f *v4, sf::Vector2f *v5)
{
    // Diagonal flip
    sf::Vector2f tmp2 = *v2;
    *v2 = *v4;
    *v4 = tmp2;
    sf::Vector2f tmp0 = *v0;
    *v0 = *v3;
    *v3 = tmp0;
    sf::Vector2f tmp1 = *v1;
    *v1 = *v5;
    *v5 = tmp1;
}

void Chunk::doFlips(uint8_t bits, sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2,
                    sf::Vector2f *v3, sf::Vector2f *v4, sf::Vector2f *v5)
{
    // 0000 = no change
    // 0100 = vertical = swap y axis
    // 1000 = horizontal = swap x axis
    // 1100 = horiz + vert = swap both axes = horiz+vert = rotate 180 degrees
    // 0010 = diag = rotate 90 degrees right and swap x axis
    // 0110 = diag+vert = rotate 270 degrees right
    // 1010 = horiz+diag = rotate 90 degrees right
    // 1110 = horiz+vert+diag = rotate 90 degrees right and swap y axis
    if (!(bits & tmx::TileLayer::FlipFlag::Horizontal)
        && !(bits & tmx::TileLayer::FlipFlag::Vertical)
        && !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // Shortcircuit tests for nothing to do
        return;
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal)
             && (bits & tmx::TileLayer::FlipFlag::Vertical)
             && !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 0100
        flipY(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal)
             && !(bits & tmx::TileLayer::FlipFlag::Vertical)
             && !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 1000
        flipX(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal)
             && (bits & tmx::TileLayer::FlipFlag::Vertical)
             && !(bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 1100
        flipY(v0, v1, v2, v3, v4, v5);
        flipX(v0, v1, v2, v3, v4, v5);
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal)
             && !(bits & tmx::TileLayer::FlipFlag::Vertical)
             && (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 0010
        flipD(v0, v1, v2, v3, v4, v5);
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal)
             && (bits & tmx::TileLayer::FlipFlag::Vertical)
             && (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 0110
        flipX(v0, v1, v2, v3, v4, v5);
        flipD(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal)
             && !(bits & tmx::TileLayer::FlipFlag::Vertical)
             && (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 1010
        flipY(v0, v1, v2, v3, v4, v5);
        flipD(v0, v1, v2, v3, v4, v5);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal)
             && (bits & tmx::TileLayer::FlipFlag::Vertical)
             && (bits & tmx::TileLayer::FlipFlag::Diagonal))
    {
        // 1110
        flipY(v0, v1, v2, v3, v4, v5);
        flipX(v0, v1, v2, v3, v4, v5);
        flipD(v0, v1, v2, v3, v4, v5);
    }
}

void Chunk::draw(sf::RenderTarget &rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (const auto &a : _chunkArrays)
    {
        rt.draw(*a, states);
    }
}

void Chunk::generateTiles(bool registerAnimation)
{
    if (registerAnimation)
    {
        _activeAnimations.clear();
    }
    for (const auto &ca : _chunkArrays)
    {
        std::uint32_t idx = 0;
        std::uint32_t xPos = static_cast<std::uint32_t>(getPosition().x / _mapTileSize.x);
        std::uint32_t yPos = static_cast<std::uint32_t>(getPosition().y / _mapTileSize.y);
        for (auto y = yPos; y < yPos + _chunkTileCount.y; ++y)
        {
            for (auto x = xPos; x < xPos + _chunkTileCount.x; ++x)
            {
                if (idx < _chunkTileIDs.size() && _chunkTileIDs[idx].ID >= ca->firstGridId()
                    && _chunkTileIDs[idx].ID <= ca->lastGridId())
                {
                    if (registerAnimation
                        && _animTiles.find(_chunkTileIDs[idx].ID) != _animTiles.end())
                    {
                        AnimationState as;
                        as.animTile = _animTiles[_chunkTileIDs[idx].ID];
                        as.startTime = sf::milliseconds(0);
                        as.tileCords = sf::Vector2u(x, y);
                        _activeAnimations.push_back(as);
                    }

                    sf::Vector2f tileOffset(static_cast<float>(x) * _mapTileSize.x,
                                            static_cast<float>(y) * _mapTileSize.y + _mapTileSize.y
                                                - ca->tilesetSize().y);

                    auto idIndex = _chunkTileIDs[idx].ID - ca->firstGridId();
                    sf::Vector2f tileIndex(
                        sf::Vector2i(idIndex % ca->tileCount().x, idIndex / ca->tileCount().x));
                    tileIndex.x *= ca->tilesetSize().x;
                    tileIndex.y *= ca->tilesetSize().y;
                    ChunkArray::Tile tile{
                        sf::Vertex(tileOffset - getPosition(), sf::Color::White, tileIndex),
                        sf::Vertex(
                            tileOffset - getPosition()
                                + sf::Vector2f(static_cast<float>(ca->tilesetSize().x), 0.f),
                            sf::Color::White,
                            tileIndex + sf::Vector2f(static_cast<float>(ca->tilesetSize().x), 0.f)),
                        sf::Vertex(tileOffset - getPosition()
                                       + sf::Vector2f(
                                           sf::Vector2u(ca->tilesetSize().x, ca->tilesetSize().y)),
                                   sf::Color::White,
                                   tileIndex
                                       + sf::Vector2f(
                                           sf::Vector2u(ca->tilesetSize().x, ca->tilesetSize().y))),
                        sf::Vertex(tileOffset - getPosition(), sf::Color::White, tileIndex),
                        sf::Vertex(tileOffset - getPosition()
                                       + sf::Vector2f(
                                           sf::Vector2u(ca->tilesetSize().x, ca->tilesetSize().y)),
                                   sf::Color::White,
                                   tileIndex
                                       + sf::Vector2f(
                                           sf::Vector2u(ca->tilesetSize().x, ca->tilesetSize().y))),
                        sf::Vertex(tileOffset - getPosition()
                                       + sf::Vector2f(0.f, static_cast<float>(ca->tilesetSize().y)),
                                   sf::Color::White,
                                   tileIndex
                                       + sf::Vector2f(0.f, static_cast<float>(ca->tilesetSize().y)))
                    };
                    doFlips(_chunkTileIDs[idx].flipFlags, &tile[0].texCoords, &tile[1].texCoords,
                            &tile[2].texCoords, &tile[3].texCoords, &tile[4].texCoords,
                            &tile[5].texCoords);
                    ca->addTile(tile);
                }
                idx++;
            }
        }
    }
}

} // namespace Game::Level
