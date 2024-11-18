#include "mainwindow.h"

#include "layout/verticallayout.h"

#include "graphics-elements/button/textbutton.h"

#include "event/keyevents/keypressevent.h"
#include "event/keyevents/keyreleaseevent.h"
#include "event/mouseevents/mousepressevent.h"
#include "event/mouseevents/mousereleaseevent.h"

#include "geometry/rect.h"
#include "geometry/utils.h"

#include "iupdatable.h"
#include "iview.h"

#include "view/menuview.h"

#include "menu/menu.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <Resources/ResourceManager.h>
#include <iostream>

MainWindow::MainWindow(unsigned int width, unsigned int height, const char *name)
    : sf::RenderWindow{ sf::VideoMode({ width, height }), name },
      _menuView{ std::make_shared<MenuView>(
          this, RectF{ { 0.f, 0.f }, { static_cast<float>(width), static_cast<float>(height) } },
          this) },
      _menu{ std::make_shared<Menu>(this, this) },
      _latestMouseMoveEvent{ {}, {} },
      _renderer{ _menu },
      _view{ _menuView }
{
    ResourseManager::getInstance()->loadAllTextures();

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

        _view->update(deltatime);
        _renderer->update(deltatime);

        display();
    }

    return 0;
}

void MainWindow::keyPressEvent(KeyPressEvent *event)
{
    if (event->key() == Keyboard::Key::Escape)
        switchContent();
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
    auto layout{ std::make_unique<VerticalLayout>(
        static_cast<RectF>(Geometry::toRect(getViewport(getView()))), this) };
    layout->setSpacing(20);

    sf::Font font;
    bool successLoad{ font.loadFromFile("fonts/arial.ttf") };
    if (!successLoad)
        std::cerr << "GameMenu: Failed load font." << std::endl;

    auto startButton{ std::make_shared<Scene::TextButton>("Start Game", font,
                                                          SizeF{ 180.0f, 50.0f }, this) };
    auto exitButton{ std::make_shared<Scene::TextButton>("Exit", font, SizeF{ 180.0f, 50.0f },
                                                         this) };

    startButton->onClick([this]() { switchContent(); });
    exitButton->onClick([this]() { close(); });

    layout->addItem(startButton);
    layout->addItem(exitButton);

    _menu->setLayout(std::move(layout));
}

void MainWindow::switchContent()
{
    _showMenu = !_showMenu;

    if (_showMenu)
        switchToMenu();
    else
        switchToGame();
}

void MainWindow::switchToGame()
{
}

void MainWindow::switchToMenu()
{
    _renderer = _menu;
    _view = _menuView;
}
