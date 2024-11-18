#include "keypressevent.h"

KeyPressEvent::KeyPressEvent(Keyboard::Key key, bool shift, bool alt, bool control) : _key{ key }
{
    _mode |= shift ? Keyboard::Mode::Shift : Keyboard::Mode(0);
    _mode |= alt ? Keyboard::Mode::Alt : Keyboard::Mode(0);
    _mode |= control ? Keyboard::Mode::Control : Keyboard::Mode(0);
}

Keyboard::Key KeyPressEvent::key() const
{
    return _key;
}

Keyboard::Mode KeyPressEvent::mode() const
{
    return _mode;
}
