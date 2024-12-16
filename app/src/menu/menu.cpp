#include "menu.h"

#include "actionvariant.h"
#include "button/textbutton.h"
#include "controller.h"
#include "item/abstractitem.h"
#include "layout/verticallayout.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Menu
{

namespace
{
const sf::Vector2f VIEW_OFFSET{ 60, 0 };
}

Menu::Menu(sf::RenderTarget *renderTarget, EventHandler *parent)
    : IView{ renderTarget, parent },
      _title{ std::make_unique<sf::Text>() },
      _renderTarget{ renderTarget },
      _view{ std::make_unique<sf::View>(
          sf::FloatRect(VIEW_OFFSET,
                        sf::Vector2f(renderTarget->getSize().x, renderTarget->getSize().y))) },
      _layout{ std::make_unique<VerticalLayout>(sf::FloatRect{ VIEW_OFFSET, _view->getSize() }) },
      _levelController{ std::make_unique<Game::Level::Controller>(renderTarget, nullptr,
                                                                  "level/menu.tmx") }
{
    init();
}

Menu::~Menu() = default;

void Menu::update(float deltatime)
{
    using Item = std::shared_ptr<Graphics::AbstractItem>;

    IView::update(deltatime);

    _renderTarget->clear(sf::Color(50, 56, 59, 255));

    _levelController->update(deltatime);

    for (const Item &item : _layout->items())
        _renderTarget->draw(*item);

    _renderTarget->draw(*_title);
}

sf::View *Menu::view() const
{
    return _view.get();
}

void Menu::init()
{
    const sf::Vector2f viewSize = _view->getSize();
    const sf::Vector2f viewCenter = _view->getCenter();

    _title->setFont(ResourseManager::getInstance()->getFont(FontType::Arial));
    _title->setCharacterSize(30);
    _title->setFillColor(sf::Color::Cyan);
    _title->setCharacterSize(70);
    _title->setString("New Year Chaos");

    _title->setPosition({ viewCenter.x - _title->getGlobalBounds().width / 2,
                          viewCenter.y - (viewCenter.y / 2 + _title->getGlobalBounds().height) });

    _layout->setSpacing(20);

    const sf::Font font{ ResourseManager::getInstance()->getFont(FontType::Arial) };

    auto startButton{ std::make_shared<Graphics::TextButton>("Start Game", font,
                                                             sf::Vector2f{ 180.0f, 50.0f }, this) };
    auto exitButton{ std::make_shared<Graphics::TextButton>("Exit", font,
                                                            sf::Vector2f{ 180.0f, 50.0f }, this) };

    startButton->onClick([this]() { executeActions(ActionVariant::StartGame); });
    exitButton->onClick([this]() { executeActions(ActionVariant::Exit); });

    _layout->addItem(startButton);
    _layout->addItem(exitButton);
}

} // namespace Menu
