#pragma once

#include "action/actionstorage.h"
#include "actionvariant.h"
#include "eventhandler.h"
#include "iview.h"
#include "util/limitedvalue.h"

#include <SFML/Graphics/Text.hpp>

#include <memory>

class Layout;
class MusicController;

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
                  const sf::Vector2f &viewSize, std::shared_ptr<MusicController> soundController);
    ~Menu();

    void update(float deltatime) override;
    void updateViewSize(const sf::Vector2f &size) override;

    void updateMenuLayout(const MenuType type) override;

protected:
    sf::View *view() const override;

private:
    void initLooseLayout();
    void initDefaultLayout();

    Game::Level::Controller *levelControllerBy(MenuType type);

    void updateBackground(float deltatime);

    sf::Text _title;

    sf::RenderTarget *_renderTarget;
    std::unique_ptr<sf::View> _view;

    std::unique_ptr<Layout> _looseLayout;
    std::unique_ptr<Layout> _defaultLayout;

    Layout *_currentLayout;

    std::unique_ptr<Game::Level::Controller> _defaultLevelController;
    std::unique_ptr<Game::Level::Controller> _winLevelController;
    std::unique_ptr<Game::Level::Controller> _loseLevelController;

    Game::Level::Controller *_currentLevelController{ nullptr };

    Util::LimitedValueF _jumpTimer;
    Util::LimitedValueF _shootTimer;

    std::shared_ptr<MusicController> _soundController;
};

} // namespace Menu
