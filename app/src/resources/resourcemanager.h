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

enum class SoundType
{
    Background_music
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
    Tea,
    Health_bar,
    Stamina_icon,
    HeartBar_icon,
    FreezBar_icon,

    // Bot
    Viking_walk,
    Viking_health_bar,

    Snowman_1_idle,
    Snowman_1_hurt,
    Snowman_1_dead,
    Snowman_1_walk,
    Snowman_1_throw,

    Snowman_2_idle,
    Snowman_2_hurt,
    Snowman_2_dead,
    Snowman_2_walk,
    Snowman_2_throw,

    Snowman_3_idle,
    Snowman_3_hurt,
    Snowman_3_dead,
    Snowman_3_walk,
    Snowman_3_throw,

    // bullets
    Axe,
    SnowBall,

    // Zones
    Dead_water_zone,

    Key,

    SnowBallGun,
    Door,
    Bridge,
    Chain,
    Stone,
};

class ResourseManager
{
    using TexturePack = std::vector<sf::Texture>;

public:
    static ResourseManager *getInstance();

    void loadResourses();
    [[nodiscard]] sf::Font &getFont(const FontType type);
    [[nodiscard]] TexturePack &getTextures(const TextureType type);
    [[nodiscard]] std::filesystem::path &getSoundPath(const SoundType type);

private:
    ResourseManager() = default;

    void loadAllFonst();
    void loadAllSounds();
    void loadAllTextures();

    void loadSounds(const std::filesystem::path &path, const SoundType type,
                    const std::string &fileNamePart);
    void loadSound(const std::filesystem::path &filePath, const SoundType type);

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
    std::unordered_map<SoundType, std::filesystem::path> _sounds;
};
