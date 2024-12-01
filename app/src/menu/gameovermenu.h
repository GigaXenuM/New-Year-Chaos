#pragma once

#include "actionvariant.h"
#include "iview.h"
#include "layout/layout.h"

#include <action/actionstorage.h>
#include <memory>

class GameOverMenu : public IView, public Action::ActionStorage<Menu::ActionVariant>
{
public:
    explicit GameOverMenu(sf::RenderTarget *renderTarget, EventHandler *parent);
    //~GameOverMenu();

    void update(float deltatime) override;

protected:
    sf::View *view() const override;

private:
    void init();

    sf::RenderTarget *_renderTarget;
    std::unique_ptr<sf::View> _view;
    std::unique_ptr<Layout> _layout;
};
