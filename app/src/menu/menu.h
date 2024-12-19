#pragma once

#include "action/actionstorage.h"
#include "actionvariant.h"
#include "eventhandler.h"
#include "iview.h"
#include "util/limitedvalue.h"

#include <SFML/Graphics/Text.hpp>

#include <memory>

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
    explicit Menu(sf::RenderTarget *renderTarget, EventHandler *parent,
                  const sf::Vector2f &viewSize);
    ~Menu();

    void update(float deltatime) override;
    void updateViewSize(const sf::Vector2f &size) override;

protected:
    sf::View *view() const override;

private:
    void init();

    void updateBackground(float deltatime);

    std::unique_ptr<sf::Text> _title;

    sf::RenderTarget *_renderTarget;
    std::unique_ptr<sf::View> _view;
    std::unique_ptr<Layout> _layout;
    std::unique_ptr<Game::Level::Controller> _levelController;

    Util::LimitedValueF _jumpTimer;
    Util::LimitedValueF _shootTimer;
};

} // namespace Menu
