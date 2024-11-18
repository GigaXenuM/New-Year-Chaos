#pragma once

#include <filesystem>

#include <unordered_map>
#include <vector>

namespace sf
{
class Texture;
}

enum class TextureType
{
    Player_run,
    Player_idle,
    Player_jump,
    Player_dead,
    Player_walk,
    Player_slide
};

struct TexturePack
{
    std::vector<sf::Texture> _textures;
};

class ResourseManager
{
public:
    static ResourseManager *getInstance();

    void loadAllTextures();

private:
    ResourseManager() = default;

    void loadTexture(const std::string &filePath, const TextureType type);
    void loadTextures(const std::filesystem::path &path, const TextureType type,
                      const std::string &fileNamepart);

private:
    std::unordered_map<TextureType, TexturePack> _textures;
};
