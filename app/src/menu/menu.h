#pragma once

#include "action/actionstorage.h"
#include "eventhandler.h"
#include "iview.h"

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

namespace Menu
{

class Menu : public IView, public Action::ActionStorage
{
public:
    explicit Menu(sf::RenderTarget *renderTarget, EventHandler *parent);
    ~Menu();

    void update(float deltatime) override;
    sf::View *view() const override;

private:
    void init();

    sf::RenderTarget *_renderTarget;
    std::unique_ptr<sf::View> _view;
    std::unique_ptr<Layout> _layout;
};

} // namespace Menu
