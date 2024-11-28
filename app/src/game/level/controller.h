#pragma once

#include "SFML/System/Vector2.hpp"
#include "iupdatable.h"

#include "contact/contactlistener.h"

#include <box2d/b2_body.h>

#include <memory>
#include <vector>

class Player;
class b2World;

namespace sf
{
class View;
class RenderTarget;
class Shape;
} // namespace sf

namespace Graphics
{
class Drawable;
} // namespace Graphics

namespace Game::Level
{

class TileLayer;
class ObjectLayer;

class Controller : public IUpdatable
{
public:
    explicit Controller(sf::RenderTarget *renderTarget, sf::View *view, EventHandler *parent);
    ~Controller();

    void update(float deltatime) override;

protected:
    void keyPressEvent(KeyPressEvent *event) override;
    void keyReleaseEvent(KeyReleaseEvent *event) override;

private:
    void loadLevel();
    void initPhisicalWorld();
    void initPlayer();

    void updateCameraPos();

    sf::RenderTarget *_renderTarget{ nullptr };
    sf::View *_gameView{ nullptr };

    sf::Vector2f _safeZoneSize;
    sf::Vector2f _halfSafeZone;

    std::vector<std::unique_ptr<TileLayer>> _tileLayers;
    std::unique_ptr<ObjectLayer> _objectLayer;

    std::unique_ptr<ContactListener> _contactListener;

    std::unique_ptr<b2World> _phisicalWorld;
    std::unique_ptr<Player> _player;

    std::vector<Graphics::Drawable *> _itemsToDrawing;
};

} // namespace Game::Level
