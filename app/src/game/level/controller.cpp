#include "controller.h"

#include "action/iaction.h"
#include "action/iactionhandler.h"
#include "items/background/background.h"
#include "items/bridge/bridge.h"
#include "items/bridge/bridgedrawablejoint.h"
#include "items/bridge/openbridgeactionitem.h"
#include "items/colliderfactory.h"

#include "items/deadzone/waterzone.h"
#include "items/door/door.h"
#include "items/loot/healthloot.h"
#include "items/loot/tealoot.h"
#include "items/mountain/mountain.h"
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

#include <items/deadzone/winzome.h>

namespace
{

constexpr float TIME_STEP = 1.0f / 100.0f;
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

Controller::Controller(sf::RenderTarget *renderTarget, EventHandler *parent,
                       std::string levelSource, const sf::View *view, bool menuMode)
    : EventHandler{ parent },
      _renderTarget{ renderTarget },
      _physicalWorld{ std::make_unique<b2World>(b2Vec2(0.f, 3.8f)) },
      _mapGlobalRect{ { 0.f, 0.f }, { 0.f, 0.f } },
      _view{ view },
      _menuMode{ menuMode }
{
    loadLevel(std::move(levelSource));
    initPhisicalWorld();

    initPlayer();

    initEnemies();
    initLoot();

    initDeadZone();
    initWinZone();
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

Player *Controller::player() const
{
    return _player;
}

sf::FloatRect Controller::mapGlobalRect() const
{
    return _mapGlobalRect;
}

void Controller::keyPressEvent(KeyPressEvent *event)
{
    if (event->key() == sf::Keyboard::D)
        _player->updateState(Player::State::Right, true);
    if (event->key() == sf::Keyboard::A)
        _player->updateState(Player::State::Left, true);
    if (event->key() == sf::Keyboard::LShift)
        _player->updateState(Player::State::Run, true);
    if (event->key() == sf::Keyboard::Space)
        _player->updateState(Player::State::Jump, true);
}

void Controller::keyReleaseEvent(KeyReleaseEvent *event)
{
    if (event->key() == sf::Keyboard::D)
        _player->updateState(Player::State::Right, false);
    if (event->key() == sf::Keyboard::A)
        _player->updateState(Player::State::Left, false);
    if (event->key() == sf::Keyboard::LShift)
        _player->updateState(Player::State::Run, false);
    if (event->key() == sf::Keyboard::Space)
        _player->updateState(Player::State::Jump, false);
    if (event->key() == sf::Keyboard::E)
        executeAvailableActions();
    if (event->key() == sf::Keyboard::Q)
        _player->restoreFreezePoints();
    if (event->key() == sf::Keyboard::F)
        _player->restoreHealthPoints();
}

void Controller::mousePressEvent(MousePressEvent *event)
{
    if (event->button() == Mouse::Button::Left)
        _player->shoot(event->position());
}

void Controller::loadLevel(std::string levelSource)
{
    _independentElements.clear();

    tmx::Map map;
    map.load(levelSource);
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
            _independentElements.insert(
                { Depth::PrefixPlayer,
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

    // Background
    {
        _independentElements.insert({ Depth::Backgrouund, std::make_unique<Background>(_view) });
    }

    // Terrain
    {
        std::vector<sf::Shape *> terrainShapes{ _objectLayer->objects("terrain") };
        assert(!terrainShapes.empty());
        b2Body *terrainBody{ ColliderFactory::create<ItemType::Terrain>(_physicalWorld.get(),
                                                                        terrainShapes) };
        auto *terrainItem{ new StaticElement{ terrainBody, ItemType::Terrain } };
        _independentElements.insert(
            { Depth::PrefixPlayer, std::unique_ptr<Graphics::Drawable>{ terrainItem } });

        _mapGlobalRect = Util::convertBodyToSFMLShape(terrainBody).getGlobalBounds();
    }

    // Obstacles
    std::vector<sf::Shape *> terrainObstacleShapes{ _objectLayer->objects("terrain_obstacle") };
    for (auto *shape : terrainObstacleShapes)
    {
        b2Body *terrainObstacleBody{
            ColliderFactory::create<ItemType::TerrainObstacle>(_physicalWorld.get(), { shape })
        };
        auto *terrainObstackleItem{ new StaticElement{ terrainObstacleBody,
                                                       ItemType::TerrainObstacle } };
        _independentElements.insert(
            { Depth::PrefixPlayer, std::unique_ptr<Graphics::Drawable>{ terrainObstackleItem } });
    }

    // Obstacle actions
    std::vector<sf::Shape *> ObstacleActionShapes{ _objectLayer->objects("obstacle_action") };
    for (auto *shape : ObstacleActionShapes)
    {
        _independentElements.insert(
            { Depth::PrefixPlayer, std::make_unique<Mountain>(_physicalWorld.get(), shape) });
    }

    // Bridge
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

        _independentElements.insert({ Depth::PrefixPlayer, std::unique_ptr<Bridge>(bridge) });
        _independentElements.insert(
            { Depth::PrefixPlayer, std::unique_ptr<StaticElement>(bridgeSupport) });
        _independentElements.insert({ Depth::PostfixPlayer, std::unique_ptr<Graphics::Drawable>(
                                                                bridge->postDrawElement()) });
        _independentElements.insert(
            { Depth::PostfixPlayer,
              std::make_unique<OpenBridgeActionItem>(_physicalWorld.get(), &openActionShape,
                                                     bridge) });
    }

    // Doors
    const auto &doorContainer{ _objectLayer->objects("door") };
    for (auto *shape : doorContainer)
    {
        _independentElements.insert(
            { Depth::PostfixPlayer, std::make_unique<Door>(_physicalWorld.get(), shape) });
    }

    // Dead Zones
    const auto &deadZoneContainer{ _objectLayer->objects("dead_zone") };
    for (auto *shape : deadZoneContainer)
    {
        b2Body *body{ ColliderFactory::create<ItemType::DeadZone>(_physicalWorld.get(),
                                                                  { shape }) };
        _independentElements.insert(
            { Depth::PrefixPlayer, std::make_unique<StaticElement>(body, ItemType::DeadZone) });
    }

    // Warm Zones
    const auto &warmZoneContainer{ _objectLayer->objects("warm_zone") };
    for (auto *shape : warmZoneContainer)
    {
        b2Body *body{ ColliderFactory::create<ItemType::WarmZone>(_physicalWorld.get(),
                                                                  { shape }) };
        _independentElements.insert(
            { Depth::PrefixPlayer, std::make_unique<StaticElement>(body, ItemType::WarmZone) });
    }
}

void Controller::initPlayer()
{
    const auto &playerContainer{ _objectLayer->objects("player") };
    assert(playerContainer.size() == 1);
    sf::Shape *playerShape{ playerContainer.front() };

    _player = new Player(_physicalWorld.get(), playerShape, _menuMode);
    _independentElements.insert({ Depth::Player, std::unique_ptr<Player>(_player) });
    _dependedElements.insert({ Depth::Hint, _player->hint() });
}

void Controller::initEnemies()
{
    const auto &itemContainer{ _objectLayer->objects("snowman_1") };
    const auto &itemContainer2{ _objectLayer->objects("snowman_2") };
    const auto &itemContainer3{ _objectLayer->objects("snowman_3") };

    for (auto *shape : itemContainer3)
    {
        _independentElements.insert(
            { Depth::PrefixPlayer, std::make_unique<Bot3>(_physicalWorld.get(), shape, _player) });
    }

    for (auto *shape : itemContainer2)
    {
        _independentElements.insert(
            { Depth::PrefixPlayer, std::make_unique<Bot2>(_physicalWorld.get(), shape, _player) });
    }

    for (auto *shape : itemContainer)
    {
        _independentElements.insert(
            { Depth::PrefixPlayer, std::make_unique<Bot>(_physicalWorld.get(), shape, _player) });
    }
}

void Controller::initDeadZone()
{
    const auto &itemContainer{ _objectLayer->objects("water_place") };

    for (auto *shape : itemContainer)
    {
        _independentElements.insert(
            { Depth::PrefixPlayer, std::make_unique<WaterZone>(_physicalWorld.get(), shape) });
    }
}

void Controller::initWinZone()
{
    const auto &itemContainer{ _objectLayer->objects("win_zone") };

    for (auto *shape : itemContainer)
    {
        _independentElements.insert(
            { Depth::PrefixPlayer, std::make_unique<WinZone>(_physicalWorld.get(), shape) });
    }
}

void Controller::initLoot()
{
    const auto &itemContainer{ _objectLayer->objects("tea") };

    for (auto *shape : itemContainer)
    {
        auto *lootItem = new TeaLoot{ _physicalWorld.get(), shape };
        lootItem->setCallback([lootItem]() { lootItem->prepareDestroy(); });
        _independentElements.insert({ Depth::PrefixPlayer, std::unique_ptr<TeaLoot>{ lootItem } });
    }

    const auto &aidContainer{ _objectLayer->objects("aid") };

    for (auto *shape : aidContainer)
    {
        auto *lootItem = new HealthLoot{ _physicalWorld.get(), shape };
        _independentElements.insert(
            { Depth::PrefixPlayer, std::unique_ptr<HealthLoot>{ lootItem } });
    }
}

void Controller::updatePhysics(float deltatime)
{
    _timeAccumulator.update(deltatime);

    while (_timeAccumulator.resolve(TIME_STEP))
    {
        _physicalWorld->Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

        for (const auto &[_, item] : _independentElements)
        {
            auto *physicalItem{ dynamic_cast<AbstractPhysicalItem *>(item.get()) };
            if (physicalItem != nullptr)
                physicalItem->updatePhysics();
        }
    }
}

void Controller::updateGraphics(float deltatime)
{
    for (const auto &[_, item] : _independentElements)
        item->update(deltatime);
}

void Controller::render(float deltatime)
{
    auto mergedElements{ _dependedElements };
    for (auto &[key, uniqueDrawable] : _independentElements)
        mergedElements.emplace(key, uniqueDrawable.get());

    for (const auto &[_, item] : mergedElements)
        _renderTarget->draw(*item);

    // _physicalWorld->DebugDraw();
}

void Controller::destroyRedundantItems()
{
    for (auto &[_, item] : _independentElements)
    {
        if (auto abstractPhysicalItem{ dynamic_cast<AbstractPhysicalItem *>(item.get()) })
        {
            if (abstractPhysicalItem->needDestroying())
            {
                for (auto *drop : abstractPhysicalItem->dropLoots())
                    _independentElements.insert(
                        { Depth::PostfixPlayer, std::unique_ptr<AbstractPhysicalItem>(drop) });
                abstractPhysicalItem->destroyCollider();
                item.reset();
            }
        }
    }

    for (auto it = _independentElements.begin(); it != _independentElements.end();)
        it = (it->second == nullptr) ? _independentElements.erase(it) : std::next(it);
}

void Controller::visitActions()
{
    std::vector<IAction *> actions;
    std::vector<IActionHandler *> actionHandlers;
    actions.reserve(_independentElements.size());
    actionHandlers.reserve(_independentElements.size());

    for (auto &[_, item] : _independentElements)
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
    actionHandlers.reserve(_independentElements.size());

    for (auto &[_, item] : _independentElements)
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
