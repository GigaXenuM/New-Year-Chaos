#include "controller.h"

#include "action/iaction.h"
#include "action/iactionhandler.h"
#include "items/bridge/bridge.h"
#include "items/bridge/bridgedrawablejoint.h"
#include "items/bridge/openbridgeactionitem.h"
#include "items/colliderfactory.h"

#include "items/deadzone/waterzone.h"
#include "items/loot/tealoot.h"
#include "keyevents/keypressevent.h"
#include "keyevents/keyreleaseevent.h"
#include "mouseevents/mousepressevent.h"

#include "contact/contactlistener.h"
#include "item/drawable.h"
#include "object-layer/objectlayer.h"
#include "player/bot2.h"
#include "player/player.h"
#include "tile-layer/tilelayer.h"
#include "util/debugdrawer.h"
#include "util/geometryoperation.h"

#include "player/bot.h"

#include <box2d/b2_math.h>
#include <box2d/b2_world.h>

#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>

#include <player/bot3.h>

namespace
{
constexpr unsigned PREFIX_DRAW{ 0 };
constexpr unsigned PLAYER_DRAW{ 1 };
constexpr unsigned POSTFIX_DRAW{ 2 };

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

    ItemType type() const override
    {
        return _type;
    }

    bool needDestroying() const override
    {
        return false;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
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
      _physicalWorld{ std::make_unique<b2World>(b2Vec2(0.f, 3.8f)) }
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
    visitActions();

    destroyRedundantItems();
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
            _elements.insert(
                { PREFIX_DRAW,
                  std::make_unique<TileLayer>(map, layer->getLayerAs<tmx::TileLayer>()) });
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
    _physicalWorld->SetAllowSleeping(true);
    _physicalWorld->SetDebugDraw(debugDraw);
    _physicalWorld->SetContactListener(ContactListener::instance());
    debugDraw->SetFlags(b2Draw::e_shapeBit);

    std::vector<sf::Shape *> terrainShapes{ _objectLayer->objects("terrain") };
    b2Body *terrainBody{ ColliderFactory::create<ItemType::Terrain>(_physicalWorld.get(),
                                                                    terrainShapes) };
    auto *terrainItem{ new StaticElement{ terrainBody, ItemType::Terrain } };

    std::vector<sf::Shape *> terrainObstacleShapes{ _objectLayer->objects("terrain_obstacle") };
    b2Body *terrainObstacleBody{ ColliderFactory::create<ItemType::TerrainObstacle>(
        _physicalWorld.get(), terrainObstacleShapes) };
    auto *terrainObstackleItem{ new StaticElement{ terrainObstacleBody,
                                                   ItemType::TerrainObstacle } };

    _elements.insert({ PREFIX_DRAW, std::unique_ptr<Graphics::Drawable>{ terrainItem } });
    _elements.insert({ PREFIX_DRAW, std::unique_ptr<Graphics::Drawable>{ terrainObstackleItem } });

    const auto &bridgeContainer{ _objectLayer->objects("bridge") };
    for (auto *shape : bridgeContainer)
    {
        sf::RectangleShape bridgeSupportShape{ { 30, 30 } };
        bridgeSupportShape.setOrigin(
            Util::pointBy(bridgeSupportShape.getLocalBounds(), Util::ALIGN_CENTER_STATE));
        bridgeSupportShape.setPosition(
            Util::pointBy(shape->getGlobalBounds(), { Align::Right, Align::Bottom }));
        b2Body *body{ ColliderFactory::create<ItemType::DeadZone>(_physicalWorld.get(),
                                                                  { &bridgeSupportShape }) };

        auto *bridgeSupport{ new StaticElement(body, ItemType::NonCollided) };
        auto *bridge{ new Bridge{ _physicalWorld.get(), shape } };

        b2RevoluteJointDef jointDef;
        jointDef.bodyA = body;
        jointDef.bodyB = bridge->collider();
        jointDef.enableLimit = true;
        jointDef.lowerAngle = 0;
        jointDef.upperAngle = b2_pi / 2;

        sf::Vector2f anchorA{ Util::pointBy(bridgeSupportShape.getGlobalBounds(),
                                            Util::ALIGN_CENTER_STATE) };
        sf::Vector2f anchorB{ Util::pointBy(shape->getGlobalBounds(),
                                            { Align::Right, Align::Bottom }) };

        jointDef.localAnchorA.Set(anchorA.x, anchorA.y);
        jointDef.localAnchorB.Set(anchorB.x, anchorB.y);
        jointDef.collideConnected = false;
        _physicalWorld->CreateJoint(&jointDef);

        sf::RectangleShape openActionShape{ { 64, 64 } };

        _elements.insert({ PREFIX_DRAW, std::unique_ptr<Bridge>(bridge) });
        _elements.insert({ PREFIX_DRAW, std::unique_ptr<StaticElement>(bridgeSupport) });
        _elements.insert(
            { POSTFIX_DRAW, std::unique_ptr<Graphics::Drawable>(bridge->postDrawElement()) });
        _elements.insert(
            { POSTFIX_DRAW, std::make_unique<OpenBridgeActionItem>(_physicalWorld.get(),
                                                                   &openActionShape, bridge) });
    }

    const auto &deadZoneContainer{ _objectLayer->objects("dead_zone") };
    for (auto *shape : deadZoneContainer)
    {
        b2Body *body{ ColliderFactory::create<ItemType::DeadZone>(_physicalWorld.get(),
                                                                  { shape }) };
        _elements.insert(
            { PREFIX_DRAW, std::make_unique<StaticElement>(body, ItemType::DeadZone) });
    }
}

void Controller::initPlayer()
{
    const auto &playerContainer{ _objectLayer->objects("player") };
    assert(playerContainer.size() == 1);
    sf::Shape *playerShape{ playerContainer.front() };

    gPlayer = new Player(_physicalWorld.get(), playerShape);
    _elements.insert({ PLAYER_DRAW, std::unique_ptr<Player>(gPlayer) });
}

void Controller::initBot()
{
    const auto &itemContainer{ _objectLayer->objects("snowman_1") };
    const auto &itemContainer2{ _objectLayer->objects("snowman_2") };
    const auto &itemContainer3{ _objectLayer->objects("snowman_3") };

    for (auto *shape : itemContainer3)
    {
        _elements.insert({ PREFIX_DRAW, std::make_unique<Bot3>(_physicalWorld.get(), shape) });
    }

    for (auto *shape : itemContainer2)
    {
        _elements.insert({ PREFIX_DRAW, std::make_unique<Bot2>(_physicalWorld.get(), shape) });
    }

    for (auto *shape : itemContainer)
    {
        _elements.insert({ PREFIX_DRAW, std::make_unique<Bot>(_physicalWorld.get(), shape) });
    }
}

void Controller::initDeadZone()
{
    const auto &itemContainer{ _objectLayer->objects("water_place") };

    for (auto *shape : itemContainer)
    {
        _elements.insert({ PREFIX_DRAW, std::make_unique<WaterZone>(_physicalWorld.get(), shape) });
    }
}

void Controller::initLoot()
{
    const auto &itemContainer{ _objectLayer->objects("tea") };

    for (auto *shape : itemContainer)
    {
        auto *lootItem = new TeaLoot{ _physicalWorld.get(), shape };
        lootItem->setCallback([lootItem]() { lootItem->prepareDestroy(); });
        _elements.insert({ PREFIX_DRAW, std::unique_ptr<TeaLoot>{ lootItem } });
    }
}

void Controller::updatePhysics(float deltatime)
{
    _timeAccumulator.update(deltatime);

    while (_timeAccumulator.resolve(TIME_STEP))
    {
        _physicalWorld->Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

        for (const auto &[_, item] : _elements)
        {
            auto *physicalItem{ dynamic_cast<AbstractPhysicalItem *>(item.get()) };
            if (physicalItem != nullptr)
                physicalItem->updatePhysics();
        }
    }
}

void Controller::updateGraphics(float deltatime)
{
    for (const auto &[_, item] : _elements)
        item->update(deltatime);

    updateCameraPos();
}

void Controller::render(float deltatime)
{
    for (const auto &[_, item] : _elements)
        _renderTarget->draw(*item);

    _physicalWorld->DebugDraw();
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

void Controller::destroyRedundantItems()
{
    for (auto &[_, item] : _elements)
    {
        if (auto abstractPhysicalItem{ dynamic_cast<AbstractPhysicalItem *>(item.get()) })
        {
            if (abstractPhysicalItem->needDestroying())
            {
                for (auto *drop : abstractPhysicalItem->dropLoots())
                    _elements.insert({ POSTFIX_DRAW, std::unique_ptr<AbstractPhysicalItem>(drop) });
                abstractPhysicalItem->destroyCollider();
                item.reset();
            }
        }
    }

    for (auto it = _elements.begin(); it != _elements.end();)
        it = (it->second == nullptr) ? _elements.erase(it) : std::next(it);
}

void Controller::visitActions()
{
    std::vector<IAction *> actions;
    std::vector<IActionHandler *> actionHandlers;
    actions.reserve(_elements.size());
    actionHandlers.reserve(_elements.size());

    for (auto &[_, item] : _elements)
    {
        auto *action{ dynamic_cast<IAction *>(item.get()) };
        if (action != nullptr)
            actions.push_back(action);
        auto *actionHandler{ dynamic_cast<IActionHandler *>(item.get()) };
        if (actionHandler != nullptr)
            actionHandlers.push_back(actionHandler);
    }

    for (auto *actionHandler : actionHandlers)
        actionHandler->visitActions(actions);
}

void Controller::executeAvailableActions()
{
    std::vector<IActionHandler *> actionHandlers;
    actionHandlers.reserve(_elements.size());

    for (auto &[_, item] : _elements)
    {
        auto *actionHandler{ dynamic_cast<IActionHandler *>(item.get()) };
        if (actionHandler != nullptr)
            actionHandlers.push_back(actionHandler);
    }

    for (auto *actionHandler : actionHandlers)
        actionHandler->executeAvailableAction();
}

} // namespace Level
} // namespace Game
