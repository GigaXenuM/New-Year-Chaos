#include "eventhandler.h"

#include "event.h"
#include "event/keyevents/keypressevent.h"
#include "event/keyevents/keyreleaseevent.h"
#include "event/mouseevents/mousemoveevent.h"
#include "event/mouseevents/mousepressevent.h"
#include "event/mouseevents/mousereleaseevent.h"

EventHandler::EventHandler(EventHandler *parent)
{
    if (parent != nullptr)
        parent->addEventHandler(this);
}

void EventHandler::handleEvent(Event *event)
{
    switch (event->type())
    {
    case Event::EventType::KEY_PRESS:
        keyPressEvent(dynamic_cast<KeyPressEvent *>(event));
        break;
    case Event::EventType::KEY_RELEASE:
        keyReleaseEvent(dynamic_cast<KeyReleaseEvent *>(event));
        break;
    case Event::EventType::MOUSE_MOVE:
        mouseMoveEvent(dynamic_cast<MouseMoveEvent *>(event));
        break;
    case Event::EventType::MOUSE_PRESS:
        mousePressEvent(dynamic_cast<MousePressEvent *>(event));
        break;
    case Event::EventType::MOUSE_RELEASE:
        mouseReleaseEvent(dynamic_cast<MouseReleaseEvent *>(event));
        break;
    case Event::EventType::MOUSE_SCROLL:
        break;
    }

    for (auto *handler : _handlers)
        handler->handleEvent(event);
}

void EventHandler::addEventHandler(EventHandler *handler)
{
    _handlers.push_back(handler);
}

void EventHandler::removeEventHandler(EventHandler *handler)
{
    auto toRemove{ std::find(_handlers.begin(), _handlers.end(), handler) };
    _handlers.erase(toRemove);
}

void EventHandler::keyPressEvent(KeyPressEvent * /*event*/)
{
    // Override it to define the logic.
}

void EventHandler::keyReleaseEvent(KeyReleaseEvent * /*event*/)
{
    // Override it to define the logic.
}

void EventHandler::mousePressEvent(MousePressEvent * /*event*/)
{
    // Override it to define the logic.
}

void EventHandler::mouseReleaseEvent(MouseReleaseEvent *event)
{
    // Override it to define the logic.
}

void EventHandler::mouseMoveEvent(MouseMoveEvent *event)
{
    // Override it to define the logic.
}
