#pragma once

#include <cassert>
#include <vector>

class Event;
class KeyPressEvent;
class MousePressEvent;
class MouseReleaseEvent;
class KeyReleaseEvent;
class MouseMoveEvent;
class MouseScrollEvent;

class EventHandler
{
public:
    EventHandler(EventHandler *parent = nullptr);
    virtual ~EventHandler() = default;

    EventHandler(const EventHandler &other) = delete;
    EventHandler &operator=(const EventHandler &other) = delete;

    EventHandler(EventHandler &&other) = delete;
    EventHandler &operator=(EventHandler &&other) = delete;

    virtual void handleEvent(Event *event);

    void addEventHandler(EventHandler *handler);
    void removeEventHandler(EventHandler *handler);

    void grabContext(const EventHandler *handler);

    void releaseContext();

protected:
    virtual void keyPressEvent(KeyPressEvent *event);
    virtual void keyReleaseEvent(KeyReleaseEvent *event);
    virtual void mousePressEvent(MousePressEvent *event);
    virtual void mouseReleaseEvent(MouseReleaseEvent *event);
    virtual void mouseMoveEvent(MouseMoveEvent *event);
    virtual void mouseScrollEvent(MouseScrollEvent *event);

private:
    EventHandler *_parent{ nullptr };
    std::vector<EventHandler *> _children;
    EventHandler *_grabbed{ nullptr };
};
