#pragma once

#include "event/eventhandler.h"
#include "hud/hudcomponents.h"
#include "iview.h"

#include "util/enumflag.h"

#include <memory>

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
    explicit Scene(sf::RenderTarget *renderTarget, EventHandler *parent);
    virtual ~Scene();

    void update(float deltatime) override;
    sf::View *view() const override;

    bool isGameOver() const;

protected:
    void mousePressEvent(MousePressEvent *event) override;
    void mouseReleaseEvent(MouseReleaseEvent *event) override;
    void mouseMoveEvent(MouseMoveEvent *event) override;
    void mouseScrollEvent(MouseScrollEvent *event) override;

private:
    void updateCamera();

    sf::RenderTarget *_renderTarget{ nullptr };
    float _scaling{ 1.0f };
    const sf::Vector2f _viewSize;
    std::unique_ptr<sf::View> _view;
    std::unique_ptr<Level::Controller> _levelController;
    std::unique_ptr<HUDComponents> _hudComponents;

    sf::Vector2f _safeZoneSize;
    sf::Vector2f _halfSafeZone;

    Util::EnumFlag<State> _sceneState;
};

} // namespace Game
