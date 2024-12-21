#include "mainwindow.h"

#include "layout/verticallayout.h"

#include "graphics-element/button/textbutton.h"

#include "event/keyevents/keypressevent.h"
#include "event/keyevents/keyreleaseevent.h"
#include "event/mouseevents/mousepressevent.h"
#include "event/mouseevents/mousereleaseevent.h"
#include "event/mouseevents/mousescrollevent.h"

#include "iview.h"

#include "menu/actionvariant.h"
#include "menu/menu.h"

#include "game/scene.h"
#include "musiccontroller.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace
{
constexpr float DEFAULT_WIDTH{ 1400 };
sf::Vector2f defineViewSize(const sf::Vector2f &windowSize)
{
    const float scaling{ DEFAULT_WIDTH / windowSize.x };
    return { windowSize.x * scaling, windowSize.y * scaling };
}
} // namespace

MainWindow::MainWindow(unsigned int width, unsigned int height, const char *name)
    : sf::RenderWindow{ sf::VideoMode({ width, height }), name },
      EventHandler{ nullptr },
      _viewSize{ defineViewSize(sf::Vector2f(width, height)) },
      _soundController{ std::make_shared<MusicController>() },
      _menu{ std::make_unique<Menu::Menu>(this, this, _viewSize, _soundController) },
      _scene{ std::make_unique<Game::Scene>(this, this, _viewSize, _soundController) },
      _latestMouseMoveEvent{ {}, {} }
{
    composeMenu();

    switchView();
}

MainWindow::~MainWindow()
{
}

int MainWindow::gameLoop()
{
    while (isOpen())
    {
        float deltatime{ _clock.restart().asSeconds() };

        sf::Event event;
        while (pollEvent(event))
            handleSfmlEvent(event);

        _currentView->update(deltatime);
        if (_scene->isGameOver())
        {
            _menu->updateMenuLayout(MenuType::GameOver);
            switchView(_menu.get());
        }
        if (_scene->isPlayerWon())
        {
            _menu->updateMenuLayout(MenuType::Victory);
            _scene = std::make_unique<Game::Scene>(this, this, _viewSize, _soundController);
            switchView(_menu.get());
        }

        display();
    }

    return 0;
}

void MainWindow::keyPressEvent(KeyPressEvent *event)
{
    if (event->key() == Keyboard::Key::Escape)
        switchView();
}

void MainWindow::handleSfmlEvent(const sf::Event &event)
{
    switch (event.type)
    {
    case sf::Event::Closed:
    {
        close();
        break;
    }
    case sf::Event::KeyPressed:
    {
        KeyPressEvent e{ Keyboard::Key(event.key.code), event.key.shift, event.key.alt,
                         event.key.control };
        handleEvent(&e);
        break;
    }
    case sf::Event::KeyReleased:
    {
        KeyReleaseEvent e{ Keyboard::Key(event.key.code), event.key.shift, event.key.alt,
                           event.key.control };
        handleEvent(&e);
        break;
    }
    case sf::Event::MouseButtonPressed:
    {
        sf::Vector2f position{ mapPixelToCoords(sf::Mouse::getPosition(*this), this->getView()) };
        MousePressEvent e{ Mouse::Button(event.mouseButton.button), position };

        handleEvent(&e);
        break;
    }
    case sf::Event::MouseButtonReleased:
    {
        sf::Vector2f position{ mapPixelToCoords(sf::Mouse::getPosition(*this), this->getView()) };
        MouseReleaseEvent e{ Mouse::Button(event.mouseButton.button), position };

        handleEvent(&e);
        break;
    }
    case sf::Event::MouseMoved:
    {
        MouseMoveEvent e{ { static_cast<float>(event.mouseMove.x),
                            static_cast<float>(event.mouseMove.y) },
                          _latestMouseMoveEvent.position() };
        handleEvent(&e);
        _latestMouseMoveEvent = e;
        break;
    }
    case sf::Event::MouseWheelScrolled:
    {
        MouseScrollEvent e{ event.mouseWheelScroll.delta,
                            { static_cast<float>(event.mouseWheelScroll.x),
                              static_cast<float>(event.mouseWheelScroll.y) } };
        handleEvent(&e);
        break;
    }
    default:
        break;
    }
}

void MainWindow::composeMenu()
{
    _menu->registerAction(Menu::ActionVariant::Exit, [this]() { close(); });
    _menu->registerAction(Menu::ActionVariant::StartGame,
                          [this]()
                          {
                              _menu->updateMenuLayout(MenuType::Default);
                              switchView();
                          });
    _menu->registerAction(Menu::ActionVariant::RestartGame,
                          [this]()
                          {
                              _scene = std::make_unique<Game::Scene>(this, this, _viewSize,
                                                                     _soundController);
                              _menu->updateMenuLayout(MenuType::Default);
                              switchView(_scene.get());
                          });
}

void MainWindow::switchView()
{
    const bool needShowMenu{ _currentView != _menu.get() };

    IView *view{ needShowMenu ? dynamic_cast<IView *>(_menu.get())
                              : dynamic_cast<IView *>(_scene.get()) };

    switchView(view);
}

void MainWindow::switchView(IView *view)
{
    if (view == _scene.get())
        _soundController->playMusic(MusicController::SoundAssignment::Geme);

    grabContext(view);
    _currentView = view;
}
