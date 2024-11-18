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

    void loadResourses();

private:
    ResourseManager() = default;

    void loadAllFonst();
    void loadAllTextures();

    void loadfont(const std::string &filePath, const FontType type);
    void loadFonts(const std::filesystem::path &path, const FontType type,
                   const std::string &fileNamepart);

    void loadTexture(const std::string &filePath, const TextureType type);
    void loadTextures(const std::filesystem::path &path, const TextureType type,
                      const std::string &fileNamepart);

private:
    std::unordered_map<FontType, sf::Font> _fonts;
    std::unordered_map<TextureType, TexturePack> _textures;
};
