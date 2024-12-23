#pragma once

#include "event/eventhandler.h"
#include "event/mouseevents/mousemoveevent.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

class MusicController;

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

    void switchView();
    void switchView(IView *view);

    sf::Clock _clock;

    sf::Vector2f _viewSize;

    std::shared_ptr<MusicController> _soundController;

    std::unique_ptr<Menu::Menu> _menu;
    std::unique_ptr<Game::Scene> _scene;

    IView *_currentView{ nullptr };

    MouseMoveEvent _latestMouseMoveEvent;
};
