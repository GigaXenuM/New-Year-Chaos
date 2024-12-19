#include "menu.h"

#include "actionvariant.h"
#include "button/textbutton.h"
#include "controller.h"
#include "item/abstractitem.h"
#include "layout/verticallayout.h"
#include "player/player.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Menu
{

sf::FloatRect viewRect(const sf::View *view)
{
    const sf::Vector2f center{ view->getCenter() };
    const sf::Vector2f size{ view->getSize() };
    const sf::Vector2f rectPosition{ center.x - size.x / 2.f, center.y - size.y / 2.f };

    return { rectPosition, size };
}

Menu::Menu(sf::RenderTarget *renderTarget, EventHandler *parent, const sf::Vector2f &viewSize)
    : IView{ renderTarget, parent },
      _title{ std::make_unique<sf::Text>() },
      _renderTarget{ renderTarget },
      _view{ std::make_unique<sf::View>(sf::FloatRect{ {}, viewSize }) },
      _looseLayout{ std::make_unique<VerticalLayout>(this) },
      _defaultLayout{ std::make_unique<VerticalLayout>(this) },
      _levelController{ std::make_unique<Game::Level::Controller>(renderTarget, nullptr,
                                                                  "level/menu.tmx", _view.get()) },
      _jumpTimer{ 0.f, 0.f, 3.f },
      _shootTimer{ 0.f, 0.f, 4.f }
{
    initLooseLayout();
    initDefaultLayout();

    updateMenuLayout(MenuType::Default);
}

Menu::~Menu() = default;

void Menu::update(float deltatime)
{
    using Item = std::shared_ptr<Graphics::AbstractItem>;

    IView::update(deltatime);

    _renderTarget->clear(sf::Color(50, 56, 59, 255));

    _levelController->update(deltatime);
    updateBackground(deltatime);

    for (const Item &item : _currentLayout->items())
        _renderTarget->draw(*item);

    _renderTarget->draw(*_title);
}

void Menu::updateViewSize(const sf::Vector2f &size)
{
    const sf::Vector2f mapCenter{ Util::pointBy(_levelController->mapGlobalRect(),
                                                Util::ALIGN_CENTER_STATE) };
    const sf::Vector2f playerCenter{ Util::pointBy(_levelController->player()->boundingRect(),
                                                   Util::ALIGN_CENTER_STATE) };
    _view->setSize(size);
    _view->setCenter(mapCenter.x, playerCenter.y);
    _defaultLayout->setRect(viewRect(_view.get()));
}

void Menu::updateMenuLayout(const MenuType type)
{
    switch (type)
    {
    case MenuType::Default:
        _currentLayout = _defaultLayout.get();
        break;
    case MenuType::GameOver:
        _currentLayout = _looseLayout.get();
        break;
    case MenuType::Victory:
        break;
    default:
        break;
    }

    grabContext(_currentLayout);
}

sf::View *Menu::view() const
{
    return _view.get();
}

void Menu::initLooseLayout()
{
    const sf::Vector2f mapCenter{ Util::pointBy(_levelController->mapGlobalRect(),
                                                Util::ALIGN_CENTER_STATE) };
    const sf::Vector2f playerCenter{ Util::pointBy(_levelController->player()->boundingRect(),
                                                   Util::ALIGN_CENTER_STATE) };
    _view->setCenter(mapCenter.x, playerCenter.y);
    _looseLayout->setRect(viewRect(_view.get()));

    const sf::Vector2f viewSize = _view->getSize();
    const sf::Vector2f viewCenter = _view->getCenter();

    _title->setFont(ResourseManager::getInstance()->getFont(FontType::Arial));
    _title->setCharacterSize(30);
    _title->setFillColor(sf::Color::Cyan);
    _title->setCharacterSize(70);
    _title->setString("GAME OVER!!!");

    _title->setPosition({ viewCenter.x - _title->getGlobalBounds().width / 2,
                          viewCenter.y - (viewCenter.y / 2 + _title->getGlobalBounds().height) });

    _looseLayout->setSpacing(20);

    const sf::Font font{ ResourseManager::getInstance()->getFont(FontType::Arial) };

    auto restartButton{ std::make_shared<Graphics::TextButton>("Try again", font,
                                                               sf::Vector2f{ 180.0f, 50.0f },
                                                               _looseLayout.get()) };
    auto exit{ std::make_shared<Graphics::TextButton>("Exit", font, sf::Vector2f{ 180.0f, 50.0f },
                                                      _looseLayout.get()) };

    restartButton->onClick([this]() { executeActions(ActionVariant::RestartGame); });
    exit->onClick([this]() { executeActions(ActionVariant::Exit); });

    _looseLayout->addItem(restartButton);
    _looseLayout->addItem(exit);
}

void Menu::initDefaultLayout()
{
    const sf::Vector2f mapCenter{ Util::pointBy(_levelController->mapGlobalRect(),
                                                Util::ALIGN_CENTER_STATE) };
    const sf::Vector2f playerCenter{ Util::pointBy(_levelController->player()->boundingRect(),
                                                   Util::ALIGN_CENTER_STATE) };
    _view->setCenter(mapCenter.x, playerCenter.y);
    _defaultLayout->setRect(viewRect(_view.get()));

    const sf::Vector2f viewSize = _view->getSize();
    const sf::Vector2f viewCenter = _view->getCenter();

    _title->setFont(ResourseManager::getInstance()->getFont(FontType::Arial));
    _title->setCharacterSize(30);
    _title->setFillColor(sf::Color::Cyan);
    _title->setCharacterSize(70);
    _title->setString("New Year Chaos");

    _title->setPosition({ viewCenter.x - _title->getGlobalBounds().width / 2,
                          viewCenter.y - (viewCenter.y / 2 + _title->getGlobalBounds().height) });

    _defaultLayout->setSpacing(20);

    const sf::Font font{ ResourseManager::getInstance()->getFont(FontType::Arial) };

    auto startButton{ std::make_shared<Graphics::TextButton>("Start Game", font,
                                                             sf::Vector2f{ 180.0f, 50.0f },
                                                             _defaultLayout.get()) };
    auto exitButton{ std::make_shared<Graphics::TextButton>("Exit", font,
                                                            sf::Vector2f{ 180.0f, 50.0f },
                                                            _defaultLayout.get()) };

    startButton->onClick([this]() { executeActions(ActionVariant::StartGame); });
    exitButton->onClick([this]() { executeActions(ActionVariant::Exit); });

    _defaultLayout->addItem(startButton);
    _defaultLayout->addItem(exitButton);
}

void Menu::updateBackground(float deltatime)
{
    using PlayerState = Game::PhysicalEntity::State;

    _jumpTimer.move(deltatime);
    const bool needJump{ _jumpTimer.isMax() };
    if (needJump)
        _jumpTimer.setMin();

    _shootTimer.move(deltatime);
    const bool needShoot{ _shootTimer.isMax() };
    if (needShoot)
        _shootTimer.setMin();

    Game::Player *player{ _levelController->player() };
    player->updateState(PlayerState::Jump, needJump);
    if (needShoot)
    {

        const sf::Vector2f targetPos{ Util::pointBy(player->boundingRect(),
                                                    { Align::Top, Align::Right }) };
        player->shoot(targetPos);
    }
}

} // namespace Menu
