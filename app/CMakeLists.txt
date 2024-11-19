cmake_minimum_required(VERSION 3.20)

set(HEADER_FILES
    src/mainwindow.h

    src/iview.h
    src/iupdatable.h

    src/scene/scene.h

    src/visitor/ivisitor.h
    src/visitor/defaultvisitor.h

    src/resources/resourceManager.h

    src/graphics-element/item/drawable.h
    src/graphics-element/player/player.h
    src/graphics-element/layout/layout.h
    src/graphics-element/button/ibutton.h
    src/graphics-element/item/abstractitem.h
    src/graphics-element/button/textbutton.h
    src/graphics-element/layout/verticallayout.h

    src/event/event.h
    src/event/eventhandler.h
    src/event/keyevents/keyboard.h
    src/event/keyevents/keypressevent.h
    src/event/keyevents/keyreleaseevent.h
    src/event/mouseevents/mousepressevent.h
    src/event/mouseevents/mousemoveevent.h
    src/event/mouseevents/mousereleaseevent.h
    src/event/mouseevents/mouse.h

    src/geometry/point.h
    src/geometry/rect.h
    src/geometry/size.h
    src/geometry/utils.h
    src/geometry/alignment.h

    src/menu/menu.h
    src/menu/actionvariant.h

    src/action/actionstorage.h
)

set(SOURCE_FILES
    src/main.cpp

    src/mainwindow.cpp

    src/scene/scene.cpp

    src/visitor/defaultvisitor.cpp

    src/resources/resourceManager.cpp

    src/graphics-element/player/player.cpp
    src/graphics-element/layout/layout.cpp
    src/graphics-element/button/textbutton.cpp
    src/graphics-element/item/abstractitem.cpp
    src/graphics-element/layout/verticallayout.cpp

    src/event/eventhandler.cpp
    src/event/keyevents/keypressevent.cpp
    src/event/keyevents/keyreleaseevent.cpp
    src/event/mouseevents/mousepressevent.cpp
    src/event/mouseevents/mousemoveevent.cpp
    src/event/mouseevents/mousereleaseevent.cpp

    src/geometry/utils.cpp

    src/menu/menu.cpp
)

include_directories(
    "${CMAKE_SOURCE_DIR}/app/src"
    "${CMAKE_SOURCE_DIR}/app/src/scene"
    "${CMAKE_SOURCE_DIR}/app/src/geometry"
    "${CMAKE_SOURCE_DIR}/app/src/menu"
    "${CMAKE_SOURCE_DIR}/app/src/graphics-element"
    "${CMAKE_SOURCE_DIR}/app/src/event"
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} )

target_link_libraries(${PROJECT_NAME} sfml-graphics)

target_sources(New-Year-Chaos
  PRIVATE
    src/graphics-element/player/player.h src/graphics-element/player/player.cpp
)
target_link_libraries(${PROJECT_NAME} sfml-audio)
