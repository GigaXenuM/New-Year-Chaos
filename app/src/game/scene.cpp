#include "scene.h"

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
      _levelController{ std::make_unique<Level::Controller>(renderTarget, this,
                                                            "level/terrain.tmx") },
      _hudComponents{ std::make_unique<HUDComponents>(_renderTarget, _view.get(),
                                                      _levelController->player()) },
      _safeZoneSize{ _view->getSize().x * 0.2f, _view->getSize().y * 0.2f },
      _halfSafeZone{ _safeZoneSize / 2.f }
{
}

Scene::~Scene() = default;

void Scene::update(float deltatime)
{
    IView::update(deltatime);

    _renderTarget->clear(sf::Color(125, 117, 138, 255));

    _levelController->update(deltatime);
    _hudComponents->update(deltatime);

    updateCamera();
}

sf::View *Scene::view() const
{
    return _view.get();
}

bool Scene::isGameOver() const
{
    return _levelController->player()->isDead();
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
    const float delta{ rawDelta * 0.1f };
    _scaling -= delta;

    _view->setSize(_viewSize * _scaling);
}

void Scene::updateCamera()
{
    const Player *player{ _levelController->player() };
    const sf::Vector2f playerPosition = player->getPosition();

    sf::Vector2f viewCenter{ _view->getCenter() };
    const sf::Vector2f safeZoneMin{ viewCenter.x - _halfSafeZone.x,
                                    viewCenter.y - _halfSafeZone.y };
    const sf::Vector2f safeZoneMax{ viewCenter.x + _halfSafeZone.x,
                                    viewCenter.y + _halfSafeZone.y };

    if (playerPosition.x < safeZoneMin.x)
        viewCenter.x = playerPosition.x + _halfSafeZone.x;
    else if (playerPosition.x > safeZoneMax.x)
        viewCenter.x = playerPosition.x - _halfSafeZone.x;
    if (playerPosition.y < safeZoneMin.y)
        viewCenter.y = playerPosition.y + _halfSafeZone.y;
    else if (playerPosition.y > safeZoneMax.y)
        viewCenter.y = playerPosition.y - _halfSafeZone.y;

    _view->setCenter(viewCenter);
}

} // namespace Game
