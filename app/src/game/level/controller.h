#pragma once

#include "SFML/System/Vector2.hpp"
#include "iupdatable.h"

#include "contact/contactlistener.h"
#include "player/bot.h"
#include "util/timeaccumulator.h"

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
    void initBot();

    void calculate(float deltatime);
    void render(float deltatime);

    void updateCameraPos();

    sf::RenderTarget *_renderTarget{ nullptr };
    sf::View *_gameView{ nullptr };

    sf::Vector2f _safeZoneSize;
    sf::Vector2f _halfSafeZone;

    std::unique_ptr<ObjectLayer> _objectLayer;

    std::unique_ptr<ContactListener> _contactListener;

    std::unique_ptr<b2World> _phisicalWorld;
    Player *_player;
    Bot *_bot;

    Util::TimeAccumulator _timeAccumulator;

    std::vector<std::unique_ptr<Graphics::Drawable>> _elements;
};

} // namespace Game::Level
