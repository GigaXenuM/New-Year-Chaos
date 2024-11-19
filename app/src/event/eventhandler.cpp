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

    if (_grabbed != nullptr)
    {
        _grabbed->handleEvent(event);
        return;
    }

    for (auto *handler : _children)
        handler->handleEvent(event);
}

void EventHandler::addEventHandler(EventHandler *handler)
{
    if (handler == nullptr)
        return;

    _children.push_back(handler);

    EventHandler *handlerParent{ handler->_parent };
    if (handlerParent != nullptr)
        handlerParent->removeEventHandler(handler);

    handlerParent = this;
}

void EventHandler::removeEventHandler(EventHandler *handler)
{
    auto toRemove{ std::find(_children.begin(), _children.end(), handler) };
    _children.erase(toRemove);
}

void EventHandler::grabContext(const EventHandler *handler)
{
    auto it{ std::find(_children.begin(), _children.end(), handler) };
    if (it == _children.end())
    {
        std::cerr << "EventHandler::grabContext: The handler you want to grab not found."
                  << std::endl;
        assert(false);
        return;
    }

    _grabbed = *it;
}

void EventHandler::releaseContext()
{
    _grabbed = nullptr;
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
