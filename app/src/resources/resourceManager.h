#pragma once

#include <filesystem>

#include <unordered_map>
#include <vector>

namespace sf
{
class Font;
class Texture;
} // namespace sf

enum class FontType
{
    Arial
};

enum class TextureType
{
    // Player
    Player_run,
    Player_idle,
    Player_jump,
    Player_dead,
    Player_walk,
    Player_slide,

    // HUD
    Health_bar,
    HeartBar_icon,
    FreezBar_icon
};

class ResourseManager
{
    using TexturePack = std::vector<sf::Texture>;

public:
    static ResourseManager *getInstance();

    void loadResourses();
    [[nodiscard]] sf::Font &getFont(const FontType type);
    [[nodiscard]] TexturePack &getTextures(const TextureType type);

private:
    ResourseManager() = default;

    void loadAllFonst();
    void loadAllTextures();

    void loadfont(const std::string &filePath, const FontType type);
    void loadFonts(const std::filesystem::path &path, const FontType type,
                   const std::string &fileNamePart);

    void loadTexture(const std::string &filePath, const TextureType type);
    void loadTextures(const std::filesystem::path &path, const TextureType type,
                      const std::string &fileNamePart);

    void sortFilesByNameNumbers(std::vector<std::filesystem::path> &files);

private:
    std::unordered_map<FontType, sf::Font> _fonts;
    std::unordered_map<TextureType, TexturePack> _textures;
};
