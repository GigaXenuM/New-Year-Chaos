#include "ResourceManager.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

#include <filesystem>
#include <iostream>

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
    loadAllTextures();
}

void ResourseManager::loadAllFonst()
{
    loadFonts("fonts/", FontType::Arial, "arial");
}

void ResourseManager::loadAllTextures()
{
    loadTextures("player/png/", TextureType::Player_run, "Run");
    loadTextures("player/png/", TextureType::Player_walk, "Walk");
    loadTextures("player/png/", TextureType::Player_dead, "Dead");
    loadTextures("player/png/", TextureType::Player_jump, "Jump");
    loadTextures("player/png/", TextureType::Player_idle, "Idle");
    loadTextures("player/png/", TextureType::Player_slide, "Slide");
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

    _textures[type].emplace_back(std::move(texture));
    std::cout << "Loaded texture: " << filePath << std::endl;
}

void ResourseManager::loadTextures(const std::filesystem::path &path, const TextureType type,
                                   const std::string &fileNamePart)
{
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".png")
        {
            if (entry.path().filename().string().find(fileNamePart) == 0)
                loadTexture(entry.path().string(), type);
        }
    }
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

ResourseManager::TexturePack &ResourseManager::getTextures(const TextureType type)
{
    auto it = _textures.find(type);
    if (it == _textures.end())
    {
        throw std::runtime_error("Textures not loaded: requested TextureType does not exist");
    }
    return it->second;
}
