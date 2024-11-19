#include "mainwindow.h"

#include "layout/verticallayout.h"

#include "graphics-element/button/textbutton.h"

#include "event/keyevents/keypressevent.h"
#include "event/keyevents/keyreleaseevent.h"
#include "event/mouseevents/mousepressevent.h"
#include "event/mouseevents/mousereleaseevent.h"

#include "geometry/utils.h"

#include "iview.h"

#include "menu/actionvariant.h"
#include "menu/menu.h"

#include "scene/scene.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <Resources/ResourceManager.h>

MainWindow::MainWindow(unsigned int width, unsigned int height, const char *name)
    : sf::RenderWindow{ sf::VideoMode({ width, height }), name },
      EventHandler{ nullptr },
      _menu{ std::make_unique<Menu::Menu>(this, this) },
      _scene{ std::make_unique<Scene::Scene>(this, this) },
      _currentView{ _menu.get() },
      _latestMouseMoveEvent{ {}, {} }
{
    ResourseManager::getInstance()->loadResourses();
    composeMenu();
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
        sf::Vector2f sfmlPos{ mapPixelToCoords(sf::Mouse::getPosition(*this), this->getView()) };
        MousePressEvent e{ Mouse::Button(event.mouseButton.button), Geometry::toPoint(sfmlPos) };

        handleEvent(&e);
        break;
    }
    case sf::Event::MouseButtonReleased:
    {
        sf::Vector2f sfmlPos{ mapPixelToCoords(sf::Mouse::getPosition(*this), this->getView()) };
        MouseReleaseEvent e{ Mouse::Button(event.mouseButton.button), Geometry::toPoint(sfmlPos) };

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
    default:
        break;
    }
}

void MainWindow::composeMenu()
{
    _menu->registerAction(Menu::ActionVariant::Exit, [this]() { close(); });
    _menu->registerAction(Menu::ActionVariant::StartGame, [this]() { switchView(); });
}

void MainWindow::switchView()
{
    static bool showMenu{ true };
    showMenu = !showMenu;

    IView *view{ showMenu ? dynamic_cast<IView *>(_menu.get())
                          : dynamic_cast<IView *>(_scene.get()) };
    switchView(view);
}

void MainWindow::switchView(IView *view)
{
    grabContext(view);
    _currentView = view;
}
