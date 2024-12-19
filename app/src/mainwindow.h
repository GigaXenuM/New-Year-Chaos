#pragma once

#include "SFML/Audio/Music.hpp"
#include "event/eventhandler.h"
#include "event/mouseevents/mousemoveevent.h"
#include "gameovermenu.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

class IView;
namespace Menu
{
class Menu;
} // namespace Menu

namespace Game
{
class Scene;
} // namespace Game

class MainWindow : public sf::RenderWindow, public EventHandler
{
public:
    explicit MainWindow(unsigned int width, unsigned int height, const char *name);
    virtual ~MainWindow();

    int gameLoop();

protected:
    void keyPressEvent(KeyPressEvent *event) override;

private:
    void handleSfmlEvent(const sf::Event &event);

    void composeMenu();
    void initBackgroundMusic();

    void switchView();
    void switchView(IView *view);

    sf::Clock _clock;
    sf::Music _backgroundMusic;

    sf::Vector2f _viewSize;

    std::unique_ptr<Menu::Menu> _menu;
    std::unique_ptr<Game::Scene> _scene;

    IView *_currentView{ nullptr };

    MouseMoveEvent _latestMouseMoveEvent;
};
