#include "tilelayer.h"

#include <iostream>

namespace Scene::Level
{

TileLayer::TileLayer(const tmx::Map &map, size_t idx)
{
    const auto &layers = map.getLayers();
    if (map.getOrientation() != tmx::Orientation::Orthogonal)
    {
        std::cout << "Map is not orthogonal - nothing will be drawn" << std::endl;
    }
    else if (idx >= layers.size())
    {
        std::cout << "Layer index " << idx << " is out of range, layer count is " << layers.size()
                  << std::endl;
    }
    else if (layers[idx]->getType() != tmx::Layer::Type::Tile)
    {
        std::cout << "layer " << idx << " is not a Tile layer..." << std::endl;
    }

    else
    {
        // round the chunk size to the nearest tile
        const auto tileSize = map.getTileSize();
        _chunkSize.x = std::floor(_chunkSize.x / tileSize.x) * tileSize.x;
        _chunkSize.y = std::floor(_chunkSize.y / tileSize.y) * tileSize.y;
        _mapTileSize.x = map.getTileSize().x;
        _mapTileSize.y = map.getTileSize().y;
        const auto &layer = layers[idx]->getLayerAs<tmx::TileLayer>();
        createChunks(map, layer);

        auto mapSize = map.getBounds();
    }
}

void TileLayer::setTile(int32_t tileX, int32_t tileY, tmx::TileLayer::Tile tile, bool refresh)
{
    sf::Vector2u chunkLocale;
    const auto &selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    selectedChunk->setTile(chunkLocale.x, chunkLocale.y, tile, refresh);
}

tmx::TileLayer::Tile TileLayer::getTile(int32_t tileX, int32_t tileY)
{
    sf::Vector2u chunkLocale;
    const auto &selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    return selectedChunk->getTile(chunkLocale.x, chunkLocale.y);
}

void TileLayer::update(sf::Time elapsed)
{
    for (auto &c : _visibleChunks)
    {
        for (Scene::Level::AnimationState &as : c->getActiveAnimations())
        {
            as.currentTime += elapsed;

            tmx::TileLayer::Tile tile;
            std::int32_t animTime = 0;
            auto x = as.animTile.animation.frames.begin();
            while (animTime < as.currentTime.asMilliseconds())
            {
                if (x == as.animTile.animation.frames.end())
                {
                    x = as.animTile.animation.frames.begin();
                    as.currentTime -= sf::milliseconds(animTime);
                    animTime = 0;
                }

                tile.ID = x->tileID;
                animTime += x->duration;
                x++;
            }

            setTile(as.tileCords.x, as.tileCords.y, tile);
        }
    }
}

Chunk::Ptr &TileLayer::getChunkAndTransform(int32_t x, int32_t y, sf::Vector2u &chunkRelative)
{
    std::uint32_t chunkX = (x * _mapTileSize.x) / static_cast<std::uint32_t>(_chunkSize.x);
    std::uint32_t chunkY = (y * _mapTileSize.y) / static_cast<std::uint32_t>(_chunkSize.y);
    chunkRelative.x = ((x * _mapTileSize.x) - chunkX * static_cast<std::uint32_t>(_chunkSize.x))
                      / _mapTileSize.x;
    chunkRelative.y = ((y * _mapTileSize.y) - chunkY * static_cast<std::uint32_t>(_chunkSize.y))
                      / _mapTileSize.y;
    return _chunks[chunkX + chunkY * _chunkCount.x];
}

void TileLayer::createChunks(const tmx::Map &map, const tmx::TileLayer &layer)
{
    // look up all the tile sets and load the textures
    const auto &tileSets = map.getTilesets();
    const auto &layerIDs = layer.getTiles();
    std::uint32_t maxID = std::numeric_limits<std::uint32_t>::max();
    std::vector<const tmx::Tileset *> usedTileSets;

    for (auto i = tileSets.rbegin(); i != tileSets.rend(); ++i)
    {
        for (const auto &tile : layerIDs)
        {
            if (tile.ID >= i->getFirstGID() && tile.ID < maxID)
            {
                usedTileSets.push_back(&(*i));
                break;
            }
        }
        maxID = i->getFirstGID();
    }

    sf::Image fallback;
    fallback.create(2, 2, sf::Color::Magenta);
    for (const auto &ts : usedTileSets)
    {
        const auto &path = ts->getImagePath();

        std::unique_ptr<sf::Texture> newTexture = std::make_unique<sf::Texture>();
        sf::Image img;
        if (!img.loadFromFile(path))
        {
            newTexture->loadFromImage(fallback);
        }
        else
        {
            if (ts->hasTransparency())
            {
                auto transparency = ts->getTransparencyColour();
                img.createMaskFromColor(
                    { transparency.r, transparency.g, transparency.b, transparency.a });
            }
            newTexture->loadFromImage(img);
        }
        _textureResource.insert(std::make_pair(path, std::move(newTexture)));
    }

    // calculate the number of chunks in the layer
    // and create each one
    const auto bounds = map.getBounds();
    _chunkCount.x = static_cast<sf::Uint32>(std::ceil(bounds.width / _chunkSize.x));
    _chunkCount.y = static_cast<sf::Uint32>(std::ceil(bounds.height / _chunkSize.y));

    sf::Vector2u tileSize(map.getTileSize().x, map.getTileSize().y);

    for (auto y = 0u; y < _chunkCount.y; ++y)
    {
        sf::Vector2f tileCount(_chunkSize.x / tileSize.x, _chunkSize.y / tileSize.y);
        for (auto x = 0u; x < _chunkCount.x; ++x)
        {
            // calculate size of each Chunk (clip against map)
            if ((x + 1) * _chunkSize.x > bounds.width)
            {
                tileCount.x = (bounds.width - x * _chunkSize.x) / map.getTileSize().x;
            }
            if ((y + 1) * _chunkSize.y > bounds.height)
            {
                tileCount.y = (bounds.height - y * _chunkSize.y) / map.getTileSize().y;
            }
            // m_chunks.emplace_back(std::make_unique<Chunk>(layer, usedTileSets,
            //     sf::Vector2f(x * m_chunkSize.x, y * m_chunkSize.y), tileCount,
            //     map.getTileCount().x, m_textureResource));
            _chunks.emplace_back(std::make_unique<Chunk>(
                layer, usedTileSets, sf::Vector2f(x * _chunkSize.x, y * _chunkSize.y), tileCount,
                tileSize, map.getTileCount().x, _textureResource, map.getAnimatedTiles()));
        }
    }
}

void TileLayer::updateVisibility(const sf::View &view) const
{
    sf::Vector2f viewCorner = view.getCenter();
    viewCorner -= view.getSize() / 2.f;

    std::int32_t posX = static_cast<std::int32_t>(std::floor(viewCorner.x / _chunkSize.x));
    std::int32_t posY = static_cast<std::int32_t>(std::floor(viewCorner.y / _chunkSize.y));
    std::int32_t posX2
        = static_cast<std::int32_t>(std::ceil((viewCorner.x + view.getSize().x) / _chunkSize.x));
    std::int32_t posY2
        = static_cast<std::int32_t>(std::ceil((viewCorner.y + view.getSize().x) / _chunkSize.y));

    std::vector<Chunk *> visible;
    for (auto y = posY; y < posY2; ++y)
    {
        for (auto x = posX; x < posX2; ++x)
        {
            std::size_t idx = y * std::int32_t(_chunkCount.x) + x;
            if (idx >= 0u && idx < _chunks.size() && !_chunks[idx]->empty())
            {
                visible.push_back(_chunks[idx].get());
            }
        }
    }

    std::swap(_visibleChunks, visible);
}

void TileLayer::draw(sf::RenderTarget &rt, sf::RenderStates states) const
{
    // calc view coverage and draw nearest chunks
    updateVisibility(rt.getView());
    for (const auto &c : _visibleChunks)
        rt.draw(*c, states);
}

} // namespace Scene::Level
