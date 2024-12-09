#include "scene.h"

#include "keyevents/keypressevent.h"
#include "level/controller.h"

#include "event/mouseevents/mousescrollevent.h"
#include "mouseevents/mousemoveevent.h"
#include "mouseevents/mousepressevent.h"
#include "mouseevents/mousereleaseevent.h"
#include "player/player.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{
Scene::Scene(sf::RenderTarget *renderTarget, EventHandler *parent)
    : IView{ renderTarget, parent },
      _renderTarget{ renderTarget },
      _viewSize{ sf::Vector2f(renderTarget->getSize().x, renderTarget->getSize().y) },
      _view{ std::make_unique<sf::View>(sf::FloatRect(sf::Vector2f{}, _viewSize)) },
      _levelController{ std::make_unique<Level::Controller>(renderTarget, _view.get(), this) },
      _hudComponents{ std::make_unique<HUDComponents>(_renderTarget, _view.get()) }
{
}

Scene::~Scene() = default;

void Scene::update(float deltatime)
{
    IView::update(deltatime);

    _renderTarget->clear(sf::Color(125, 117, 138, 255));

    _levelController->update(deltatime);
    _hudComponents->update(deltatime);
}

sf::View *Scene::view() const
{
    return _view.get();
}

void Scene::mousePressEvent(MousePressEvent *event)
{
    if (event->button() == Mouse::Button::Right)
        _sceneState.set(State::DragView);
}

void Scene::mouseReleaseEvent(MouseReleaseEvent *event)
{
    if (event->button() == Mouse::Button::Right)
        _sceneState.unset(State::DragView);
}

void Scene::mouseMoveEvent(MouseMoveEvent *event)
{
    if (_sceneState.test(State::DragView))
        _view->move((event->lastPosition() - event->position()) * _scaling);
}

void Scene::mouseScrollEvent(MouseScrollEvent *event)
{
    const float rawDelta{ event->delta() };
    const float delta{ rawDelta / rawDelta - rawDelta / 10.f };
    _scaling *= delta;

    _view->setSize(_viewSize * _scaling);
}

} // namespace Game
