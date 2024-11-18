#include "keyreleaseevent.h"

KeyReleaseEvent::KeyReleaseEvent(Keyboard::Key key, bool shift, bool alt, bool control)
    : _key{ key }
{
    _mode |= shift ? Keyboard::Mode::Shift : Keyboard::Mode(0);
    _mode |= alt ? Keyboard::Mode::Alt : Keyboard::Mode(0);
    _mode |= control ? Keyboard::Mode::Control : Keyboard::Mode(0);
}

Keyboard::Key KeyReleaseEvent::key() const
{
    return _key;
}

Keyboard::Mode KeyReleaseEvent::mode() const
{
    return _mode;
}
