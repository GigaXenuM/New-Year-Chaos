cmake_minimum_required(VERSION 3.20)

set(HEADER_FILES
    src/mainwindow.h

    src/iview.h
    src/iupdatable.h

    src/graphics-elements/layout/layout.h
    src/graphics-elements/layout/verticallayout.h

    src/scene/items/drawable.h
    src/scene/items/abstractitem.h
    src/scene/visitors/ivisitor.h
    src/scene/visitors/defaultvisitor.h

    src/graphics-elements/button/ibutton.h
    src/graphics-elements/button/textbutton.h

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
    src/menu/view/menuview.h
)

set(SOURCE_FILES
    src/main.cpp

    src/mainwindow.cpp

    src/graphics-elements/layout/layout.cpp
    src/graphics-elements/layout/verticallayout.cpp

    src/scene/items/abstractitem.cpp
    src/scene/visitors/defaultvisitor.cpp

    src/graphics-elements/button/textbutton.cpp

    src/event/eventhandler.cpp
    src/event/keyevents/keypressevent.cpp
    src/event/keyevents/keyreleaseevent.cpp
    src/event/mouseevents/mousepressevent.cpp
    src/event/mouseevents/mousemoveevent.cpp
    src/event/mouseevents/mousereleaseevent.cpp

    src/geometry/utils.cpp

    src/menu/menu.cpp
    src/menu/view/menuview.cpp
)

include_directories(
    "${CMAKE_SOURCE_DIR}/app/src"
    "${CMAKE_SOURCE_DIR}/app/src/scene"
    "${CMAKE_SOURCE_DIR}/app/src/geometry"
    "${CMAKE_SOURCE_DIR}/app/src/menu"
    "${CMAKE_SOURCE_DIR}/app/src/graphics-elements"
    "${CMAKE_SOURCE_DIR}/app/src/event"
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} )

target_link_libraries(${PROJECT_NAME} sfml-graphics)
target_link_libraries(${PROJECT_NAME} sfml-audio)
