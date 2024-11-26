#pragma once

#include "event/eventhandler.h"
#include "iview.h"

#include "util/enumflag.h"

#include <memory>
#include <vector>

namespace sf
{
class RenderTarget;
} // namespace sf

namespace Graphics
{
class Drawable;
class AbstractItem;
} // namespace Graphics

namespace Scene
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
    virtual ~Scene() = default;

    void update(float deltatime) override;
    sf::View *view() const override;

protected:
    void mousePressEvent(MousePressEvent *event) override;
    void mouseReleaseEvent(MouseReleaseEvent *event) override;
    void mouseMoveEvent(MouseMoveEvent *event) override;
    void mouseScrollEvent(MouseScrollEvent *event) override;

private:
    sf::RenderTarget *_renderTarget{ nullptr };
    float _scaling{ 1.0f };
    const sf::Vector2f _viewSize;
    std::unique_ptr<sf::View> _view;
    std::unique_ptr<Level::Controller> _levelController;

    Util::EnumFlag<State> _sceneState;
};

} // namespace Scene
