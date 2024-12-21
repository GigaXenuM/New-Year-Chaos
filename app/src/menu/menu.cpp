#include "menu.h"

#include "actionvariant.h"
#include "button/textbutton.h"
#include "controller.h"
#include "item/abstractitem.h"
#include "layout/verticallayout.h"
#include "musiccontroller.h"
#include "player/player.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Menu
{

namespace
{
const sf::Color DEFAULT_TEXT_COLOR{ 54, 255, 137 };
const sf::Color LOSE_TEXT_COLOR{ 196, 22, 62 };
const sf::Color OUTLINE_COLOR{ 66, 59, 65 };
} // namespace

sf::FloatRect viewRect(const sf::View *view)
{
    const sf::Vector2f center{ view->getCenter() };
    const sf::Vector2f size{ view->getSize() };
    const sf::Vector2f rectPosition{ center.x - size.x / 2.f, center.y - size.y / 2.f };

    return { rectPosition, size };
}

Menu::Menu(sf::RenderTarget *renderTarget, EventHandler *parent, const sf::Vector2f &viewSize,
           std::shared_ptr<MusicController> soundController)
    : IView{ renderTarget, parent },
      _renderTarget{ renderTarget },
      _view{ std::make_unique<sf::View>(sf::FloatRect{ {}, viewSize }) },
      _looseLayout{ std::make_unique<VerticalLayout>(this) },
      _defaultLayout{ std::make_unique<VerticalLayout>(this) },
      _defaultLevelController{ std::make_unique<Game::Level::Controller>(renderTarget, nullptr,
                                                                         "level/menu.tmx",
                                                                         _view.get(), true) },
      _winLevelController{ std::make_unique<Game::Level::Controller>(renderTarget, nullptr,
                                                                     "level/menu_win.tmx",
                                                                     _view.get(), true) },
      _loseLevelController{ std::make_unique<Game::Level::Controller>(renderTarget, nullptr,
                                                                      "level/menu.tmx", _view.get(),
                                                                      true) },
      _jumpTimer{ 0.f, 0.f, 3.f },
      _shootTimer{ 0.f, 0.f, 4.f },
      _soundController{ std::move(soundController) }
{
    initDefaultLayout();
    initLooseLayout();

    updateMenuLayout(MenuType::Default);

    _title.setFont(ResourseManager::getInstance()->getFont(FontType::DejaVuSansBold));
    _title.setCharacterSize(30);
    _title.setCharacterSize(70);
    _title.setOutlineThickness(5);
    _title.setOutlineColor(OUTLINE_COLOR);
}

Menu::~Menu() = default;

void Menu::update(float deltatime)
{
    using Item = std::shared_ptr<Graphics::AbstractItem>;

    IView::update(deltatime);

    _renderTarget->clear(sf::Color(50, 56, 59, 255));

    _currentLevelController->update(deltatime);
    updateBackground(deltatime);

    for (const Item &item : _currentLayout->items())
        _renderTarget->draw(*item);

    const sf::Vector2f viewCenter = _view->getCenter();
    _title.setOrigin(Util::pointBy(_title.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _title.setPosition(
        { viewCenter.x, viewCenter.y - (viewCenter.y / 2 + _title.getGlobalBounds().height) });
    _renderTarget->draw(_title);
}

void Menu::updateViewSize(const sf::Vector2f &size)
{
    const sf::Vector2f mapCenter{ Util::pointBy(_defaultLevelController->mapGlobalRect(),
                                                Util::ALIGN_CENTER_STATE) };
    const sf::Vector2f playerCenter{
        Util::pointBy(_defaultLevelController->player()->boundingRect(), Util::ALIGN_CENTER_STATE)
    };
    _view->setSize(size);
    _view->setCenter(mapCenter.x, playerCenter.y);
    _defaultLayout->setRect(viewRect(_view.get()));
}

void Menu::updateMenuLayout(const MenuType type)
{
    std::string tileText;
    sf::Color titleColor;

    auto *nextLevelController{ levelControllerBy(type) };
    if (_currentLevelController == nextLevelController)
        return;

    _currentLevelController = nextLevelController;

    switch (type)
    {
    case MenuType::Default:
    {
        titleColor = DEFAULT_TEXT_COLOR;
        tileText = "НОВОРІЧНИЙ ХАОС";
        _currentLayout = _defaultLayout.get();
        _soundController->playMusic(MusicController::SoundAssignment::DefaultMenu);
        break;
    }
    case MenuType::GameOver:
    {
        _currentLayout = _looseLayout.get();
        titleColor = LOSE_TEXT_COLOR;
        tileText = "ШОСЬ МЕНІ ЗЛЕ";
        _soundController->playMusic(MusicController::SoundAssignment::LoseMenu);
        break;
    }
    case MenuType::Victory:
    {
        tileText = "НАРЕШТІ НАЙШОВ ТІ САНИ";
        titleColor = DEFAULT_TEXT_COLOR;
        _currentLayout = _defaultLayout.get();
        _soundController->playMusic(MusicController::SoundAssignment::WinMenu);
        break;
    }
    default:
        assert(false);
        break;
    }

    _title.setString(sf::String::fromUtf8(tileText.begin(), tileText.end()));
    _title.setFillColor(titleColor);
    grabContext(_currentLayout);
}

sf::View *Menu::view() const
{
    return _view.get();
}

void Menu::initLooseLayout()
{
    _loseLevelController->player()->updateState(Game::PhysicalEntity::State::Dead, true);

    const sf::Vector2f mapCenter{ Util::pointBy(_loseLevelController->mapGlobalRect(),
                                                Util::ALIGN_CENTER_STATE) };
    const sf::Vector2f playerCenter{ Util::pointBy(_loseLevelController->player()->boundingRect(),
                                                   Util::ALIGN_CENTER_STATE) };
    _view->setCenter(mapCenter.x, playerCenter.y);
    _looseLayout->setRect(viewRect(_view.get()));

    _looseLayout->setSpacing(20);

    const sf::Font font{ ResourseManager::getInstance()->getFont(FontType::Arial) };

    auto restartButton{ std::make_shared<Graphics::TextButton>("Спробувати ще раз", font,
                                                               sf::Vector2f{ 180.0f, 50.0f },
                                                               _looseLayout.get()) };
    auto exit{ std::make_shared<Graphics::TextButton>("Вихід", font, sf::Vector2f{ 180.0f, 50.0f },
                                                      _looseLayout.get()) };

    restartButton->onClick([this]() { executeActions(ActionVariant::RestartGame); });

    exit->onClick([this]() { executeActions(ActionVariant::Exit); });

    _looseLayout->addItem(restartButton);
    _looseLayout->addItem(exit);
}

void Menu::initDefaultLayout()
{
    const sf::Vector2f mapCenter{ Util::pointBy(_defaultLevelController->mapGlobalRect(),
                                                Util::ALIGN_CENTER_STATE) };
    const sf::Vector2f playerCenter{
        Util::pointBy(_defaultLevelController->player()->boundingRect(), Util::ALIGN_CENTER_STATE)
    };
    _view->setCenter(mapCenter.x, playerCenter.y);
    _defaultLayout->setRect(viewRect(_view.get()));

    _defaultLayout->setSpacing(20);

    const sf::Font font{ ResourseManager::getInstance()->getFont(FontType::Arial) };

    auto startButton{ std::make_shared<Graphics::TextButton>("Старт", font,
                                                             sf::Vector2f{ 180.0f, 50.0f },
                                                             _defaultLayout.get()) };
    auto exitButton{ std::make_shared<Graphics::TextButton>("Вихід", font,
                                                            sf::Vector2f{ 180.0f, 50.0f },
                                                            _defaultLayout.get()) };

    startButton->onClick([this]() { executeActions(ActionVariant::StartGame); });
    exitButton->onClick([this]() { executeActions(ActionVariant::Exit); });

    _defaultLayout->addItem(startButton);
    _defaultLayout->addItem(exitButton);
}

Game::Level::Controller *Menu::levelControllerBy(MenuType type)
{
    switch (type)
    {
    case MenuType::Default:
        return _defaultLevelController.get();
    case MenuType::GameOver:
        return _loseLevelController.get();
    case MenuType::Victory:
        return _winLevelController.get();
    }

    assert(false);

    return nullptr;
}

void Menu::updateBackground(float deltatime)
{
    using PlayerState = Game::PhysicalEntity::State;

    Game::Player *player{ _currentLevelController->player() };
    if (player->isStateActive(Game::PhysicalEntity::State::Dead))
        return;

    _jumpTimer.move(deltatime);
    const bool needJump{ _jumpTimer.isMax() };
    if (needJump)
        _jumpTimer.setMin();

    _shootTimer.move(deltatime);
    const bool needShoot{ _shootTimer.isMax() };
    if (needShoot)
        _shootTimer.setMin();

    player->updateState(PlayerState::Jump, needJump);
    if (needShoot)
    {

        const sf::Vector2f targetPos{ Util::pointBy(player->boundingRect(),
                                                    { Align::Top, Align::Right }) };
        player->shoot(targetPos);
    }
}

} // namespace Menu
