#include "resourcemanager.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <filesystem>
#include <iostream>
#include <regex>

ResourseManager *ResourseManager::getInstance()
{
    static std::unique_ptr<ResourseManager> _instance;
    if (_instance == nullptr)
        _instance = std::unique_ptr<ResourseManager>(new ResourseManager);

    return _instance.get();
}

void ResourseManager::loadResourses()
{
    loadAllFonst();
    loadAllSounds();
    loadAllTextures();
}

void ResourseManager::loadAllFonst()
{
    loadFonts("fonts/", FontType::Arial, "arial");
    loadFonts("fonts/", FontType::DejaVuSansBold, "DejaVuSans-Bold");
}

void ResourseManager::loadAllSounds()
{
    loadSounds("sounds/", SoundType::Background_music, "Background_music");
}

void ResourseManager::loadAllTextures()
{
    // PLayer
    loadTextures("player/png/", TextureType::Player_run, "Run");
    loadTextures("player/png/", TextureType::Player_walk, "Walk");
    loadTextures("player/png/", TextureType::Player_dead, "Dead");
    loadTextures("player/png/", TextureType::Player_jump, "Jump");
    loadTextures("player/png/", TextureType::Player_idle, "Idle");
    loadTextures("player/png/", TextureType::Player_slide, "Slide");

    // HUD
    loadTextures("hud/", TextureType::Tea, "Tea");
    loadTextures("hud/", TextureType::Health_bar, "Health");
    loadTextures("hud/", TextureType::HeartBar_icon, "Heart");
    loadTextures("hud/", TextureType::FreezBar_icon, "Ice");
    loadTextures("hud/", TextureType::Stamina_icon, "Stamina");

    // Bot
    loadTextures("bot/", TextureType::Viking_walk, "Viking");
    loadTextures("bot/", TextureType::Viking_health_bar, "BotHealthBar");

    // bullets
    loadTextures("snowball/", TextureType::SnowBall, "snowball");
    loadTextures("bot/", TextureType::Axe, "Axe");

    loadTextures("snowball/", TextureType::SnowBallGun, "SnowBallGun");

    loadTextures("snowman_1/Hurt/", TextureType::Snowman_1_hurt, "Hurt");
    loadTextures("snowman_1/Idle/", TextureType::Snowman_1_idle, "Idle");
    loadTextures("snowman_1/Dying/", TextureType::Snowman_1_dead, "Dying");
    loadTextures("snowman_1/Walking/", TextureType::Snowman_1_walk, "Walking");
    loadTextures("snowman_1/Throwing/", TextureType::Snowman_1_throw, "Throwing");

    loadTextures("snowman_2/Hurt/", TextureType::Snowman_2_hurt, "Hurt");
    loadTextures("snowman_2/Idle/", TextureType::Snowman_2_idle, "Idle");
    loadTextures("snowman_2/Dying/", TextureType::Snowman_2_dead, "Dying");
    loadTextures("snowman_2/Walking/", TextureType::Snowman_2_walk, "Walking");
    loadTextures("snowman_2/Throwing/", TextureType::Snowman_2_throw, "Throwing");

    loadTextures("snowman_3/Hurt/", TextureType::Snowman_3_hurt, "Hurt");
    loadTextures("snowman_3/Idle/", TextureType::Snowman_3_idle, "Idle");
    loadTextures("snowman_3/Dying/", TextureType::Snowman_3_dead, "Dying");
    loadTextures("snowman_3/Walking/", TextureType::Snowman_3_walk, "Walking");
    loadTextures("snowman_3/Throwing/", TextureType::Snowman_3_throw, "Throwing");

    loadTextures("dead-zones/", TextureType::Dead_water_zone, "waterzone");
    loadTextures("win-zones/", TextureType::Win_gift_zone, "Gift");
    loadTextures("key/", TextureType::Key, "key");

    loadTextures("level/door", TextureType::Door, "door");
    loadTextures("level/doorside", TextureType::DoorSide, "doorside");
    loadTextures("level/bridge", TextureType::Bridge, "bridge");
    loadTextures("level/chain", TextureType::Chain, "chain");
    loadTextures("level/stone", TextureType::Stone, "stone");
    loadTextures("level/background", TextureType::Background, "background");
}

void ResourseManager::loadSounds(const std::filesystem::path &path, const SoundType type,
                                 const std::string &fileNamePart)
{
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".mp3")
        {
            if (entry.path().filename().string().find(fileNamePart) == 0)
                loadSound(entry.path().string(), type);
        }
    }
}

void ResourseManager::loadSound(const std::filesystem::path &filePath, const SoundType type)
{
    _sounds[type] = std::move(filePath);
    std::cout << "Loaded sound path: " << filePath << std::endl;
}

void ResourseManager::loadfont(const std::string &filePath, const FontType type)
{
    sf::Font font;
    if (!font.loadFromFile(filePath))
    {
        std::cerr << "Failed to load font: " << filePath << std::endl;
        return;
    }

    _fonts[type] = std::move(font);
    std::cout << "Loaded font: " << filePath << std::endl;
}

void ResourseManager::loadFonts(const std::filesystem::path &path, const FontType type,
                                const std::string &fileNamePart)
{
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".ttf")
        {
            if (entry.path().filename().string().find(fileNamePart) == 0)
                loadfont(entry.path().string(), type);
        }
    }
}

void ResourseManager::loadTexture(const std::string &filePath, const TextureType type)
{
    sf::Texture texture;
    if (!texture.loadFromFile(filePath))
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return;
    }

    const bool needRepeat{ type == TextureType::Background };
    texture.setSmooth(true);
    texture.setRepeated(needRepeat);

    _textures[type].emplace_back(std::move(texture));
    std::cout << "Loaded texture: " << filePath << std::endl;
}

void ResourseManager::loadTextures(const std::filesystem::path &path, const TextureType type,
                                   const std::string &fileNamePart)
{
    std::vector<std::filesystem::path> files;

    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".png")
        {
            if (entry.path().filename().string().find(fileNamePart) == 0)
                files.push_back(entry.path());
        }
    }

    sortFilesByNameNumbers(files);

    for (const auto &file : files)
        loadTexture(file.string(), type);
}
void ResourseManager::sortFilesByNameNumbers(std::vector<std::filesystem::path> &files)
{
    std::sort(files.begin(), files.end(),
              [](const std::filesystem::path &a, const std::filesystem::path &b)
              {
                  auto extractNumber = [](const std::string &str) -> int
                  {
                      std::smatch match;
                      if (std::regex_search(str, match, std::regex(R"(\((\d+)\))")))
                      {
                          return std::stoi(match[1].str());
                      }
                      return 0;
                  };
                  return extractNumber(a.filename().string())
                         < extractNumber(b.filename().string());
              });
}

sf::Font &ResourseManager::getFont(const FontType type)
{
    auto it = _fonts.find(type);
    if (it == _fonts.end())
    {
        throw std::runtime_error("Font not loaded: requested FontType does not exist");
    }
    return it->second;
}

std::filesystem::path &ResourseManager::getSoundPath(const SoundType type)
{
    auto it = _sounds.find(type);
    if (it == _sounds.end())
    {
        throw std::runtime_error("Music path not loaded: requested SoundType does not exist");
    }
    return it->second;
}

ResourseManager::TexturePack &ResourseManager::getTextures(const TextureType type)
{
    auto it = _textures.find(type);
    if (it == _textures.end())
    {
        throw std::runtime_error("Textures not loaded: requested TextureType does not exist");
    }
    return it->second;
}
