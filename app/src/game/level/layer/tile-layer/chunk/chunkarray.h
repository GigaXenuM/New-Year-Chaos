#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <tmxlite/Tileset.hpp>
#include <tmxlite/Types.hpp>

#include <memory>

namespace Game
{
namespace Level
{

class ChunkArray final : public sf::Drawable
{
public:
    using Ptr = std::unique_ptr<ChunkArray>;
    using Tile = std::array<sf::Vertex, 6u>;

    explicit ChunkArray(const sf::Texture &texture, const tmx::Tileset &tileset);

    ~ChunkArray() = default;
    ChunkArray(const ChunkArray &) = delete;
    ChunkArray &operator=(const ChunkArray &) = delete;

    void reset();
    void addTile(const Tile &tile);

    sf::Vector2u textureSize() const;
    tmx::Vector2u tilesetSize() const;
    sf::Vector2u tileCount() const;
    std::uint32_t firstGridId() const;
    std::uint32_t lastGridId() const;

private:
    void draw(sf::RenderTarget &rt, sf::RenderStates states) const override;

    const sf::Texture &_texture;
    tmx::Tileset _tileset;
    std::vector<sf::Vertex> _vertices;
};

} // namespace Level
} // namespace Game
