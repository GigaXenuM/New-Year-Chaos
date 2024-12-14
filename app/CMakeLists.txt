cmake_minimum_required(VERSION 3.20)

set(HEADER_FILES
    src/mainwindow.h

    src/iview.h
    src/iupdatable.h

    src/game/scene.h
    src/game/level/controller.h
    src/game/level/layer/tile-layer/animation/animationstate.h
    src/game/level/layer/tile-layer/chunk/chunk.h
    src/game/level/layer/tile-layer/chunk/chunkarray.h
    src/game/level/layer/tile-layer/tilelayer.h
    src/game/level/layer/object-layer/objectlayer.h
    src/game/level/layer/object-layer/objectvariant.h
    src/game/contact/contactlistener.h
    src/game/player/bot.h
    src/game/player/bot2.h
    src/game/player/bot3.h
    src/game/player/player.h
    src/game/items/itemtype.h
    src/game/items/abstractphysicalitem.h
    src/game/items/entity/physicalentity.h
    src/game/items/bullet/physicalbullet.h
    src/game/items/loot/tealoot.h
    src/game/items/bridge/bridge.h
    src/game/items/colliderfactory.h
    src/game/items/bullet/snowball.h
    src/game/items/bullet/axebullet.h
    src/game/items/deadzone/waterzone.h
    src/game/hud/hudhealthbar.h
    src/game/hud/hudcomponents.h
    src/game/hud/weaponicon.h
    src/game/hud/teaicon.h
    src/game/weapon/iweapon.h
    src/game/weapon/snowballgun.h
    src/game/weapon/axegun.h
    src/game/action/iaction.h
    src/game/action/pickupteaaction.h
    src/game/action/actionvariant.h
    src/game/action/iactionhandler.h
    src/game/items/loot/keyloot.h
    src/game/items/bridge/bridgedrawablejoint.h
    src/game/items/bridge/openbridgeactionitem.h

    src/visitor/ivisitor.h
    src/visitor/defaultvisitor.h

    src/resources/resourcemanager.h

    src/graphics-element/item/drawable.h
    src/graphics-element/item/ilayoutitem.h
    src/graphics-element/layout/layout.h
    src/graphics-element/button/ibutton.h
    src/graphics-element/item/abstractitem.h
    src/graphics-element/button/textbutton.h
    src/graphics-element/layout/verticallayout.h

    src/animation/animation.h

    src/event/event.h
    src/event/eventhandler.h
    src/event/keyevents/keyboard.h
    src/event/keyevents/keypressevent.h
    src/event/keyevents/keyreleaseevent.h
    src/event/mouseevents/mousepressevent.h
    src/event/mouseevents/mousemoveevent.h
    src/event/mouseevents/mousereleaseevent.h
    src/event/mouseevents/mousescrollevent.h
    src/event/mouseevents/mouse.h

    src/game/weapon/iweapon.cpp
    src/geometry/alignment.h

    src/menu/menu.h
    src/menu/actionvariant.h

    src/action/actionstorage.h

    src/util/timeaccumulator.h
    src/menu/gameovermenu.h

    src/util/enumflag.h
    src/util/geometryoperation.h
    src/util/debugdrawer.h
    src/util/timeaccumulator.h

    src/game/tips/hint.h
)

set(SOURCE_FILES
    src/main.cpp

    src/mainwindow.cpp

    src/game/scene.cpp
    src/game/level/controller.cpp
    src/game/level/layer/tile-layer/chunk/chunk.cpp
    src/game/level/layer/tile-layer/chunk/chunkarray.cpp
    src/game/level/layer/tile-layer/tilelayer.cpp
    src/game/level/layer/object-layer/objectlayer.cpp
    src/game/contact/contactlistener.cpp
    src/game/player/bot.cpp
    src/game/player/bot2.cpp
    src/game/player/bot3.cpp
    src/game/player/player.cpp
    src/game/items/entity/physicalentity.cpp
    src/game/items/bullet/physicalbullet.cpp
    src/game/items/loot/tealoot.cpp
    src/game/items/bridge/bridge.cpp
    src/game/items/bullet/snowball.cpp
    src/game/items/bullet/axebullet.cpp
    src/game/items/deadzone/waterzone.cpp
    src/game/hud/hudhealthbar.cpp
    src/game/hud/hudcomponents.cpp
    src/game/hud/teaicon.cpp
    src/game/hud/weaponicon.cpp
    src/game/weapon/snowballgun.cpp
    src/game/weapon/axegun.cpp
    src/game/action/pickupteaaction.cpp
    src/game/items/loot/keyloot.cpp
    src/game/items/bridge/bridgedrawablejoint.cpp
    src/game/items/bridge/openbridgeactionitem.cpp

    src/game/items/colliderfactory.cpp

    src/visitor/defaultvisitor.cpp

    src/resources/resourcemanager.cpp

    src/graphics-element/layout/layout.cpp
    src/graphics-element/button/textbutton.cpp
    src/graphics-element/item/abstractitem.cpp
    src/graphics-element/layout/verticallayout.cpp

    src/animation/animation.cpp

    src/event/eventhandler.cpp
    src/event/keyevents/keypressevent.cpp
    src/event/keyevents/keyreleaseevent.cpp
    src/event/mouseevents/mousepressevent.cpp
    src/event/mouseevents/mousemoveevent.cpp
    src/event/mouseevents/mousereleaseevent.cpp
    src/event/mouseevents/mousescrollevent.cpp

    src/util/geometryoperation.cpp
    src/util/timeaccumulator.cpp

    src/menu/menu.cpp
    src/menu/gameovermenu.cpp

    src/util/debugdrawer.cpp
    src/util/geometryoperation.cpp
    src/util/timeaccumulator.cpp

    src/game/tips/hint.cpp
)

include_directories(
    "${CMAKE_SOURCE_DIR}/app/src"
    "${CMAKE_SOURCE_DIR}/app/src/game"
    "${CMAKE_SOURCE_DIR}/app/src/game/level"
    "${CMAKE_SOURCE_DIR}/app/src/game/level/layer"
    "${CMAKE_SOURCE_DIR}/app/src/geometry"
    "${CMAKE_SOURCE_DIR}/app/src/menu"
    "${CMAKE_SOURCE_DIR}/app/src/graphics-element"
    "${CMAKE_SOURCE_DIR}/app/src/event"
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-audio sfml-graphics tmxlite box2d)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/earcut/include)
