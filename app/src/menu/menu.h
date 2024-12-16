#pragma once

#include "action/actionstorage.h"
#include "actionvariant.h"
#include "eventhandler.h"
#include "iview.h"

#include <memory>

#include <SFML/Graphics/Text.hpp>

class Layout;

namespace sf
{
class RenderTarget;
class View;
} // namespace sf

namespace Graphics
{
class AbstractItem;
} // namespace Graphics

namespace Game::Level
{
class Controller;
}

namespace Menu
{

class Menu : public IView, public Action::ActionStorage<ActionVariant>
{
public:
    explicit Menu(sf::RenderTarget *renderTarget, EventHandler *parent);
    ~Menu();

    void update(float deltatime) override;

protected:
    sf::View *view() const override;

private:
    void init();

    sf::RenderTarget *_renderTarget;
    std::unique_ptr<sf::Text> _title;
    std::unique_ptr<sf::View> _view;
    std::unique_ptr<Layout> _layout;

    std::unique_ptr<Game::Level::Controller> _levelController;
};

} // namespace Menu
