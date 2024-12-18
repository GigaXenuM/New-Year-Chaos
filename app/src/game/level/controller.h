#pragma once

#include "eventhandler.h"
#include "iupdatable.h"

#include "util/timeaccumulator.h"

#include <box2d/b2_body.h>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <map>
#include <memory>
#include <string>

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

namespace Game
{
class Player;
class Bot;

namespace Level
{

class TileLayer;
class ObjectLayer;

class Controller : public IUpdatable, public EventHandler
{
    enum class Depth
    {
        BackgroundMap,
        Player,
        ForegroundMap,
        Hint,
    };

public:
    explicit Controller(sf::RenderTarget *renderTarget, EventHandler *parent,
                        std::string levelSource);
    ~Controller();

    void update(float deltatime) override;

    Player *player() const;
    sf::FloatRect mapGlobalRect() const;

protected:
    void keyPressEvent(KeyPressEvent *event) override;
    void keyReleaseEvent(KeyReleaseEvent *event) override;
    void mousePressEvent(MousePressEvent *event) override;

private:
    void loadLevel();
    void initPhisicalWorld();
    void initPlayer();
    void initEnemies();
    void initDeadZone();
    void initWinZone();

    void initLoot();

    void render(float deltatime);
    void updatePhysics(float deltatime);
    void updateGraphics(float deltatime);

    void destroyRedundantItems();

    void visitActions();
    void executeAvailableActions();

    sf::RenderTarget *_renderTarget{ nullptr };

    std::string _levelSource;

    Player *_player{ nullptr };

    std::unique_ptr<ObjectLayer> _objectLayer;

    std::unique_ptr<b2World> _physicalWorld;

    Util::TimeAccumulator _timeAccumulator;

    std::multimap<Depth, std::unique_ptr<Graphics::Drawable>> _independentElements;
    std::multimap<Depth, const Graphics::Drawable *> _dependedElements;

    sf::FloatRect _mapGlobalRect;
};

} // namespace Level
} // namespace Game
