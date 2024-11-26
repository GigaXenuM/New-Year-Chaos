#include "chunkarray.h"

namespace Scene::Level
{

ChunkArray::ChunkArray(const sf::Texture &texture, const tmx::Tileset &tileset)
    : _texture(texture), _tileset{ tileset }
{
}

void ChunkArray::reset()
{
    _vertices.clear();
}

void ChunkArray::addTile(const Tile &tile)
{
    for (const auto &v : tile)
    {
        _vertices.push_back(v);
    }
}

sf::Vector2u ChunkArray::textureSize() const
{
    return _texture.getSize();
}

tmx::Vector2u ChunkArray::tilesetSize() const
{
    return _tileset.getTileSize();
}

sf::Vector2u ChunkArray::tileCount() const
{
    return { textureSize().x / tilesetSize().x, textureSize().y / tilesetSize().y };
}

uint32_t ChunkArray::firstGridId() const
{
    return _tileset.getFirstGID();
}

uint32_t ChunkArray::lastGridId() const
{
    return _tileset.getLastGID();
}

void ChunkArray::draw(sf::RenderTarget &rt, sf::RenderStates states) const
{
    states.texture = &_texture;
    rt.draw(_vertices.data(), _vertices.size(), sf::Triangles, states);
}

} // namespace Scene::Level
