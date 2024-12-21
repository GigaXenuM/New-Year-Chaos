#pragma once

#include <SFML/Audio/Sound.hpp>

#include <unordered_map>

class MusicController
{
public:
    enum class SoundAssignment
    {
        DefaultMenu,
        WinMenu,
        LoseMenu,
        Geme,
    };

    MusicController();

    void playMusic(SoundAssignment assignment);

private:
    std::unordered_map<SoundAssignment, sf::Sound> _sounds;
};
