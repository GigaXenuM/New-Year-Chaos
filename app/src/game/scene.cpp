#include "scene.h"

#include "level/controller.h"

#include "event/mouseevents/mousescrollevent.h"

#include "player/player.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{
Scene::Scene(sf::RenderTarget *renderTarget, EventHandler *parent, const sf::Vector2f &viewSize)
    : IView{ renderTarget, parent },
      _renderTarget{ renderTarget },
      _view{ std::make_unique<sf::View>(sf::FloatRect{ {}, viewSize }) },
      _levelController{ std::make_unique<Level::Controller>(renderTarget, this, "level/terrain.tmx",
                                                            _view.get()) },
      _hudComponents{ std::make_unique<HUDComponents>(_renderTarget, _view.get(),
                                                      _levelController->player()) }
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

void Scene::updateViewSize(const sf::Vector2f &size)
{
    _view->setSize(size);
}

bool Scene::isGameOver() const
{
    return _levelController->player()->isDead();
}

bool Scene::isPlayerWon() const
{
    return _levelController->player()->isWon();
}

void Scene::mouseScrollEvent(MouseScrollEvent *event)
{
    const float rawDelta{ event->delta() };
    const float delta{ rawDelta * 0.1f };

    float scaling{ -delta * 100 };

    _view->setSize({ _view->getSize().x + scaling, _view->getSize().y + scaling });
}

void Scene::updateCamera()
{
    const sf::Vector2f safeZoneSize{ _view->getSize().x * 0.2f, _view->getSize().y * 0.2f };
    const sf::Vector2f halfSafeZone{ safeZoneSize / 2.f };

    const Player *player{ _levelController->player() };
    const sf::Vector2f playerPosition = player->getPosition();

    sf::Vector2f viewCenter{ _view->getCenter() };
    const sf::Vector2f safeZoneMin{ viewCenter.x - halfSafeZone.x, viewCenter.y - halfSafeZone.y };
    const sf::Vector2f safeZoneMax{ viewCenter.x + halfSafeZone.x, viewCenter.y + halfSafeZone.y };

    if (playerPosition.x < safeZoneMin.x)
        viewCenter.x = playerPosition.x + halfSafeZone.x;
    else if (playerPosition.x > safeZoneMax.x)
        viewCenter.x = playerPosition.x - halfSafeZone.x;
    if (playerPosition.y < safeZoneMin.y)
        viewCenter.y = playerPosition.y + halfSafeZone.y;
    else if (playerPosition.y > safeZoneMax.y)
        viewCenter.y = playerPosition.y - halfSafeZone.y;

    _view->setCenter(viewCenter);
}

} // namespace Game
