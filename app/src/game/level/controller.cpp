#include "controller.h"

#include "action/iaction.h"
#include "action/iactionhandler.h"
#include "items/colliderfactory.h"

#include "items/deadzone/waterzone.h"
#include "items/loot/tealoot.h"
#include "keyevents/keypressevent.h"
#include "keyevents/keyreleaseevent.h"
#include "mouseevents/mousepressevent.h"

#include "contact/contactlistener.h"
#include "item/drawable.h"
#include "object-layer/objectlayer.h"
#include "player/player.h"
#include "tile-layer/tilelayer.h"
#include "util/debugdrawer.h"
#include "util/geometryoperation.h"

#include "player/bot.h"

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

namespace Game
{

class StaticElement : public AbstractPhysicalItem
{
public:
    explicit StaticElement(b2Body *collider, ItemType type)
        : AbstractPhysicalItem{ collider }, _type{ type }
    {
    }

    ItemType type() const
    {
        return _type;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
    }

private:
    ItemType _type;
};

namespace Level
{

Controller::Controller(sf::RenderTarget *renderTarget, sf::View *view, EventHandler *parent)
    : EventHandler{ parent },
      _renderTarget{ renderTarget },
      _gameView{ view },
      _safeZoneSize{ view->getSize().x * 0.2f, view->getSize().y * 0.2f },
      _halfSafeZone{ _safeZoneSize / 2.f },
      _phisicalWorld{ std::make_unique<b2World>(b2Vec2(0.f, 3.8f)) }
{
    loadLevel();
    initPhisicalWorld();
    initBot();
    initLoot();
    initDeadZone();
    initPlayer();
}

Controller::~Controller() = default;

void Controller::update(float deltatime)
{
    updatePhysics(deltatime);
    updateGraphics(deltatime);
    render(deltatime);
    removeDeadItems();
}

void Controller::keyPressEvent(KeyPressEvent *event)
{
    if (event->key() == sf::Keyboard::D)
        gPlayer->updateState(Player::State::Right, true);
    if (event->key() == sf::Keyboard::A)
        gPlayer->updateState(Player::State::Left, true);
    if (event->key() == sf::Keyboard::LShift)
        gPlayer->updateState(Player::State::Run, true);
    if (event->key() == sf::Keyboard::Space)
        gPlayer->updateState(Player::State::Jump, true);
}

void Controller::keyReleaseEvent(KeyReleaseEvent *event)
{
    if (event->key() == sf::Keyboard::D)
        gPlayer->updateState(Player::State::Right, false);
    if (event->key() == sf::Keyboard::A)
        gPlayer->updateState(Player::State::Left, false);
    if (event->key() == sf::Keyboard::LShift)
        gPlayer->updateState(Player::State::Run, false);
    if (event->key() == sf::Keyboard::Space)
        gPlayer->updateState(Player::State::Jump, false);
    if (event->key() == sf::Keyboard::E)
        executeAvailableActions();
    if (event->key() == sf::Keyboard::Q)
        gPlayer->health();
}

void Controller::mousePressEvent(MousePressEvent *event)
{
    if (event->button() == Mouse::Button::Left)
        gPlayer->shoot(event->position());
}

void Controller::loadLevel()
{
    _elements.clear();

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
    _phisicalWorld->SetContactListener(ContactListener::instance());
    debugDraw->SetFlags(b2Draw::e_shapeBit);

    std::vector<sf::Shape *> terrainShapes{ _objectLayer->objects("terrain") };
    b2Body *terrainBody{ ColliderFactory::create<ItemType::Terrain>(_phisicalWorld.get(),
                                                                    terrainShapes) };
    auto *terrainItem{ new StaticElement{ terrainBody, ItemType::Terrain } };

    std::vector<sf::Shape *> terrainObstacleShapes{ _objectLayer->objects("terrain_obstacle") };
    b2Body *terrainObstacleBody{ ColliderFactory::create<ItemType::TerrainObstacle>(
        _phisicalWorld.get(), terrainObstacleShapes) };
    auto *terrainObstackleItem{ new StaticElement{ terrainObstacleBody,
                                                   ItemType::TerrainObstacle } };

    _elements.push_back(std::unique_ptr<Graphics::Drawable>{ terrainItem });
    _elements.push_back(std::unique_ptr<Graphics::Drawable>{ terrainObstackleItem });

    const auto &itemContainer{ _objectLayer->objects("dead_zone") };

    for (auto *shape : itemContainer)
    {
        b2Body *body{ ColliderFactory::create<ItemType::DeadZone>(_phisicalWorld.get(),
                                                                  { shape }) };
        _elements.push_back(std::make_unique<StaticElement>(body, ItemType::DeadZone));
    }
}

void Controller::initPlayer()
{
    const auto &playerContainer{ _objectLayer->objects("player") };
    assert(playerContainer.size() == 1);
    sf::Shape *playerShape{ playerContainer.front() };

    gPlayer = new Player(_phisicalWorld.get(), playerShape);
    _elements.push_back(std::unique_ptr<Player>(gPlayer));
}

void Controller::initBot()
{
    const auto &itemContainer{ _objectLayer->objects("snowman_1") };

    for (auto *shape : itemContainer)
    {
        _elements.push_back(std::make_unique<Bot>(_phisicalWorld.get(), shape));
    }
}

void Controller::initDeadZone()
{
    const auto &itemContainer{ _objectLayer->objects("water_place") };

    for (auto *shape : itemContainer)
    {
        _elements.push_back(std::make_unique<DeadWaterZone>(_phisicalWorld.get(), shape));
    }
}

void Controller::initLoot()
{
    const auto &itemContainer{ _objectLayer->objects("tea") };

    for (auto *shape : itemContainer)
    {
        _elements.push_back(std::make_unique<TeaLoot>(_phisicalWorld.get(), shape));
    }
}

void Controller::updatePhysics(float deltatime)
{
    _timeAccumulator.update(deltatime);

    while (_timeAccumulator.resolve(TIME_STEP))
    {
        _phisicalWorld->Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

        for (const auto &item : _elements)
        {
            auto *physicalItem{ dynamic_cast<AbstractPhysicalItem *>(item.get()) };
            if (physicalItem != nullptr)
                physicalItem->updatePhysics();
        }
    }
}

void Controller::updateGraphics(float deltatime)
{
    for (const auto &item : _elements)
        item->update(deltatime);

    updateCameraPos();
}

void Controller::render(float deltatime)
{
    for (const auto &item : _elements)
        _renderTarget->draw(*item);

    _phisicalWorld->DebugDraw();
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

void Controller::removeDeadItems()
{
    for (auto &item : _elements)
    {
        if (auto abstractPhysicalItem{ dynamic_cast<PhysicalEntity *>(item.get()) })
        {
            if (abstractPhysicalItem->isStateActive(Game::Player::State::RemoveMe))
            {
                abstractPhysicalItem->destroyCollider();
                item.reset();
            }
        }
    }

    _elements.erase(std::remove_if(_elements.begin(), _elements.end(),
                                   [](auto &bullet) { return bullet == nullptr; }),
                    _elements.end());
}

void Controller::executeAvailableActions()
{
    std::vector<IAction *> actions;
    std::vector<IActionHandler *> actionHandlers;
    actions.reserve(_elements.size());
    actionHandlers.reserve(_elements.size());

    for (auto &item : _elements)
    {
        auto *action{ dynamic_cast<IAction *>(item.get()) };
        if (action != nullptr)
            actions.push_back(action);
        auto *actionHandler{ dynamic_cast<IActionHandler *>(item.get()) };
        if (actionHandler != nullptr)
            actionHandlers.push_back(actionHandler);
    }

    for (auto *actionHandler : actionHandlers)
    {
        actionHandler->visitActions(actions);
        actionHandler->executeAvailableAction();
    }
}

} // namespace Level
} // namespace Game
