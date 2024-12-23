#pragma once

#include "event/eventhandler.h"
#include "hud/hudcomponents.h"
#include "iview.h"

#include "util/enumflag.h"

#include <memory>

class MusicController;

namespace sf
{
class RenderTarget;
} // namespace sf

namespace Graphics
{
class Drawable;
class AbstractItem;
} // namespace Graphics

namespace Game
{

namespace Level
{
class Controller;
}

enum class State
{
    DragView,
};

class Scene : public IView
{
public:
    explicit Scene(sf::RenderTarget *renderTarget, EventHandler *parent,
                   const sf::Vector2f &viewSize, std::shared_ptr<MusicController> soundController);
    virtual ~Scene();

    void update(float deltatime) override;
    sf::View *view() const override;
    void updateViewSize(const sf::Vector2f &size) override;

    bool isGameOver() const;
    bool isPlayerWon() const;

protected:
    void mouseScrollEvent(MouseScrollEvent *event) override;

private:
    void updateCamera();

    sf::RenderTarget *_renderTarget{ nullptr };
    std::unique_ptr<sf::View> _view;
    std::unique_ptr<Level::Controller> _levelController;
    std::unique_ptr<HUDComponents> _hudComponents;

    Util::EnumFlag<State> _sceneState;
    std::shared_ptr<MusicController> _soundController;
};

} // namespace Game
