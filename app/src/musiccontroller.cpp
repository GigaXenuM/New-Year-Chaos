#include "musiccontroller.h"

#include "resources/resourcemanager.h"
#include <cassert>

namespace
{
float volumeBy(MusicController::SoundAssignment assignment)
{
    using SoundFor = MusicController::SoundAssignment;

    switch (assignment)
    {
    case SoundFor::DefaultMenu:
        return 50.f;
    case SoundFor::WinMenu:
        return 60.f;
    case SoundFor::LoseMenu:
        return 60.f;
    case SoundFor::Geme:
        return 30.f;
    default:
        assert(false);
    }
    return 0.f;
}
} // namespace

MusicController::MusicController()
    : _sounds{ { SoundAssignment::Geme,
                 sf::Sound{ ResourseManager::getInstance()->getSoundBuffer(SoundType::Game) } },
               { SoundAssignment::WinMenu,
                 sf::Sound{ ResourseManager::getInstance()->getSoundBuffer(SoundType::WinMenu) } },
               { SoundAssignment::LoseMenu,
                 sf::Sound{ ResourseManager::getInstance()->getSoundBuffer(SoundType::LoseMenu) } },
               { SoundAssignment::DefaultMenu,
                 sf::Sound{
                     ResourseManager::getInstance()->getSoundBuffer(SoundType::DefaultMenu) } } }
{
    for (auto &[key, value] : _sounds)
    {
        value.setLoop(true);
        value.setVolume(volumeBy(key));
    }
}

void MusicController::playMusic(SoundAssignment assignment)
{
    for (auto &[key, value] : _sounds)
    {
        if (key == assignment)
        {
            value.play();
            continue;
        }

        value.stop();
    }
}
