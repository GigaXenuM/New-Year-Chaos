#include "controller.h"

#include "keyevents/keypressevent.h"
#include "keyevents/keyreleaseevent.h"

#include "item/drawable.h"
#include "object-layer/objectlayer.h"
#include "player/player.h"
#include "tile-layer/tilelayer.h"
#include "util/debugdrawer.h"
#include "util/geometryoperation.h"

#include <box2d/b2_math.h>
#include <box2d/b2_world.h>

#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>

namespace
{
constexpr float TIME_STEP = 1.0f / 60.0f;
constexpr int VELOCITY_ITERATIONS = 8;
constexpr int POSITION_ITERATIONS = 3;
} // namespace

namespace Game::Level
{

Controller::Controller(sf::RenderTarget *renderTarget, sf::View *view, EventHandler *parent)
    : EventHandler{ parent },
      _renderTarget{ renderTarget },
      _gameView{ view },
      _safeZoneSize{ view->getSize().x * 0.2f, view->getSize().y * 0.2f },
      _halfSafeZone{ _safeZoneSize / 2.f },
      _contactListener{ std::make_unique<ContactListener>() },
      _phisicalWorld{ std::make_unique<b2World>(b2Vec2(0.f, 4.8f)) }
{
    loadLevel();
    initPhisicalWorld();
    initPlayer();
    initBot();
}

Controller::~Controller() = default;

void Controller::update(float deltatime)
{
    calculate(deltatime);
    render(deltatime);
}

void Controller::keyPressEvent(KeyPressEvent *event)
{
    if (event->key() == sf::Keyboard::D)
        gPlayer->updateState(Player::State::Right, true);
    if (event->key() == sf::Keyboard::A)
        gPlayer->updateState(Player::State::Left, true);
    if (event->key() == sf::Keyboard::Space)
        gPlayer->updateState(Player::State::Jump, true);
}

void Controller::keyReleaseEvent(KeyReleaseEvent *event)
{
    if (event->key() == sf::Keyboard::D)
        gPlayer->updateState(Player::State::Right, false);
    if (event->key() == sf::Keyboard::A)
        gPlayer->updateState(Player::State::Left, false);
    if (event->key() == sf::Keyboard::Space)
        gPlayer->updateState(Player::State::Jump, false);
}

void Controller::loadLevel()
{
    _elements.clear();
    _contactListener->clear();

    tmx::Map map;
    map.load("level/terrain.tmx");
    if (map.getOrientation() != tmx::Orientation::Orthogonal)
    {
        std::cerr << "Map is not orthogonal - nothing will be drawn" << std::endl;
        return;
    }

    const auto &layers{ map.getLayers() };

    for (const auto &layer : layers)
    {
        switch (layer->getType())
        {
        case tmx::Layer::Type::Tile:
        {
            _elements.push_back(
                std::make_unique<TileLayer>(map, layer->getLayerAs<tmx::TileLayer>()));
            break;
        }
        case tmx::Layer::Type::Object:
        {
            assert(_objectLayer == nullptr
                   && "Incorrect map generation. Use only one object layer.");
            _objectLayer = std::make_unique<ObjectLayer>(layer->getLayerAs<tmx::ObjectGroup>());
            break;
        }
        default:
        {
            break;
        }
        }
    }
}

void Controller::initPhisicalWorld()
{
    Util::DebugDrawer *debugDraw(new Util::DebugDrawer{ _renderTarget });
    _phisicalWorld->SetAllowSleeping(true);
    _phisicalWorld->SetDebugDraw(debugDraw);
    _phisicalWorld->SetContactListener(_contactListener.get());
    debugDraw->SetFlags(b2Draw::e_shapeBit);

    b2BodyDef boundingBoxDef;
    boundingBoxDef.type = b2_staticBody;

    b2FixtureDef fixtureDefinition;
    fixtureDefinition.density = 1.0f;
    fixtureDefinition.friction = 0.3f;
    fixtureDefinition.restitution = 0.1f;

    std::vector<sf::Shape *> terrainShapes{ _objectLayer->objects("terrain") };

    boundingBoxDef.userData.pointer = static_cast<uintptr_t>(UserData::Terrain);
    b2Body *terrainBody{ _phisicalWorld->CreateBody(&boundingBoxDef) };
    for (const sf::Shape *shape : terrainShapes)
        Util::createComplexFixture(terrainBody, shape, &fixtureDefinition);

    std::vector<sf::Shape *> terrainObstacleShapes{ _objectLayer->objects("terrain_obstacle") };

    boundingBoxDef.userData.pointer = static_cast<uintptr_t>(UserData::TerrainObstacle);
    b2Body *terrainObstacleBody{ _phisicalWorld->CreateBody(&boundingBoxDef) };
    for (const sf::Shape *shape : terrainObstacleShapes)
        Util::createComplexFixture(terrainObstacleBody, shape, &fixtureDefinition);
}

void Controller::initPlayer()
{
    const auto &playerContainer{ _objectLayer->objects("player") };
    assert(playerContainer.size() == 1 && "");

    sf::Shape *playerShape{ _objectLayer->objects("player").front() };

    b2FixtureDef fixtureDefinition;
    fixtureDefinition.density = 1.0f;
    fixtureDefinition.friction = 0.0f;

    b2BodyDef bodyDefinition;
    bodyDefinition.type = b2_dynamicBody;
    bodyDefinition.fixedRotation = true;
    bodyDefinition.userData.pointer = static_cast<uintptr_t>(UserData::Player);

    b2Body *body{ _phisicalWorld->CreateBody(&bodyDefinition) };
    Util::createComplexFixture(body, playerShape, &fixtureDefinition);

    gPlayer = new Player(body);
    _elements.push_back(std::unique_ptr<Player>(gPlayer));
    _contactListener->registerAction(ActionVariant::PlayerOnGround, [player = gPlayer]()
                                     { player->updateState(Player::State::OnGround, true); });
    _contactListener->registerAction(ActionVariant::PlayerAboveGround, [player = gPlayer]()
                                     { player->updateState(Player::State::OnGround, false); });
}

void Controller::initBot()
{
    const auto &playerContainer{ _objectLayer->objects("viking_enemy") };
    assert(playerContainer.size() == 1 && "");

    sf::Shape *playerShape{ _objectLayer->objects("viking_enemy").front() };

    b2FixtureDef fixtureDefinition;
    fixtureDefinition.density = 1.0f;
    fixtureDefinition.friction = 0.0f;

    b2BodyDef bodyDefinition;
    bodyDefinition.type = b2_dynamicBody;
    bodyDefinition.fixedRotation = true;
    bodyDefinition.userData.pointer = static_cast<uintptr_t>(UserData::Bot);

    b2Body *body{ _phisicalWorld->CreateBody(&bodyDefinition) };
    Util::createComplexFixture(body, playerShape, &fixtureDefinition);

    _bot = new Bot{ body };
    _elements.push_back(std::unique_ptr<Bot>{ _bot });
}

void Controller::calculate(float deltatime)
{
    _timeAccumulator.update(deltatime);

    while (_timeAccumulator.resolve(TIME_STEP))
    {
        _phisicalWorld->Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

        for (const auto &item : _elements)
            item->update(deltatime);
    }

    updateCameraPos();
}

void Controller::render(float deltatime)
{
    for (const auto &item : _elements)
        _renderTarget->draw(*item);

    _phisicalWorld->DebugDraw();
}

void Controller::updatePlayerHealth()
{
}

void Controller::updateCameraPos()
{
    const sf::Vector2f playerPosition = gPlayer->getPosition();

    sf::Vector2f viewCenter{ _gameView->getCenter() };
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

    _gameView->setCenter(viewCenter);
}

} // namespace Game::Level
