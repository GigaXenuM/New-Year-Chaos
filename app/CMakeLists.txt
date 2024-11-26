cmake_minimum_required(VERSION 3.20)

set(HEADER_FILES
    src/mainwindow.h

    src/iview.h
    src/iupdatable.h

    src/scene/scene.h
    src/scene/level/controller.h
    src/scene/level/layer/tile-layer/animation/animationstate.h
    src/scene/level/layer/tile-layer/chunk/chunk.h
    src/scene/level/layer/tile-layer/chunk/chunkarray.h
    src/scene/level/layer/tile-layer/tilelayer.h
    src/scene/level/layer/object-layer/objectlayer.h
    src/scene/level/layer/object-layer/objectvariant.h

    src/visitor/ivisitor.h
    src/visitor/defaultvisitor.h

    src/resources/resourceManager.h

    src/graphics-element/hud/hudhealthbar.h

    src/graphics-element/item/drawable.h
    src/graphics-element/player/player.h
    src/graphics-element/item/ilayoutitem.h
    src/graphics-element/layout/layout.h
    src/graphics-element/button/ibutton.h
    src/graphics-element/item/abstractitem.h
    src/graphics-element/button/textbutton.h
    src/graphics-element/layout/verticallayout.h
    src/graphics-element/item/phisicalitem.h

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

    src/geometry/alignment.h

    src/menu/menu.h
    src/menu/actionvariant.h

    src/action/actionstorage.h

    src/util/enumflag.h
    src/util/geometryoperation.h
)

set(SOURCE_FILES
    src/main.cpp

    src/mainwindow.cpp

    src/scene/scene.cpp
    src/scene/level/controller.cpp
    src/scene/level/layer/tile-layer/chunk/chunk.cpp
    src/scene/level/layer/tile-layer/chunk/chunkarray.cpp
    src/scene/level/layer/tile-layer/tilelayer.cpp
    src/scene/level/layer/object-layer/objectlayer.cpp

    src/visitor/defaultvisitor.cpp

    src/resources/resourceManager.cpp

    src/graphics-element/hud/hudhealthbar.cpp

    src/graphics-element/player/player.cpp
    src/graphics-element/layout/layout.cpp
    src/graphics-element/button/textbutton.cpp
    src/graphics-element/item/abstractitem.cpp
    src/graphics-element/layout/verticallayout.cpp
    src/graphics-element/item/phisicalitem.cpp

    src/animation/animation.h

    src/event/eventhandler.cpp
    src/event/keyevents/keypressevent.cpp
    src/event/keyevents/keyreleaseevent.cpp
    src/event/mouseevents/mousepressevent.cpp
    src/event/mouseevents/mousemoveevent.cpp
    src/event/mouseevents/mousereleaseevent.cpp
    src/event/mouseevents/mousescrollevent.cpp

    src/menu/menu.cpp
        src/animation/Animation.cpp
        src/animation/Animation.h
)

include_directories(
    "${CMAKE_SOURCE_DIR}/app/src"
    "${CMAKE_SOURCE_DIR}/app/src/scene"
    "${CMAKE_SOURCE_DIR}/app/src/scene/level"
    "${CMAKE_SOURCE_DIR}/app/src/scene/level/layer"
    "${CMAKE_SOURCE_DIR}/app/src/geometry"
    "${CMAKE_SOURCE_DIR}/app/src/menu"
    "${CMAKE_SOURCE_DIR}/app/src/graphics-element"
    "${CMAKE_SOURCE_DIR}/app/src/event"
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} sfml-graphics)

target_sources(New-Year-Chaos
  PRIVATE
    src/graphics-element/player/player.h src/graphics-element/player/player.cpp
)
target_link_libraries(${PROJECT_NAME} sfml-audio)
target_link_libraries(${PROJECT_NAME} tmxlite)
