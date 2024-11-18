#pragma once

#include <vector>

class Event;
class KeyPressEvent;
class MousePressEvent;
class MouseReleaseEvent;
class KeyReleaseEvent;
class MouseMoveEvent;

class EventHandler
{
public:
    virtual ~EventHandler() = default;

    virtual void handleEvent(Event *event);

    void addEventHandler(EventHandler *handler);
    void removeEventHandler(EventHandler *handler);

protected:
    virtual void keyPressEvent(KeyPressEvent *event);
    virtual void keyReleaseEvent(KeyReleaseEvent *event);
    virtual void mousePressEvent(MousePressEvent *event);
    virtual void mouseReleaseEvent(MouseReleaseEvent *event);
    virtual void mouseMoveEvent(MouseMoveEvent *event);

private:
    std::vector<EventHandler *> _handlers;
};
