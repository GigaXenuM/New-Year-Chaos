#include "menu.h"

#include "actionvariant.h"
#include "button/textbutton.h"
#include "item/abstractitem.h"
#include "layout/verticallayout.h"
#include <resources/resourcemanager.h>

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace Menu
{

Menu::Menu(sf::RenderTarget *renderTarget, EventHandler *parent)
    : IView{ renderTarget, parent },
      _renderTarget{ renderTarget },
      _view{ std::make_unique<sf::View>(
          sf::FloatRect(sf::Vector2f{},
                        sf::Vector2f(renderTarget->getSize().x, renderTarget->getSize().y))) },
      _layout{ std::make_unique<VerticalLayout>(sf::FloatRect{ {}, _view->getSize() }) }
{
    init();
}

Menu::~Menu() = default;

void Menu::update(float deltatime)
{
    using Item = std::shared_ptr<Graphics::AbstractItem>;

    IView::update(deltatime);

    _renderTarget->clear(sf::Color(50, 56, 59, 255));

    for (const Item &item : _layout->items())
        _renderTarget->draw(*item);
}

sf::View *Menu::view() const
{
    return _view.get();
}

void Menu::init()
{
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
