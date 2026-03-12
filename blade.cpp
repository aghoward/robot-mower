#include "blade.h"

void Blade::handle(bool state_changed)
{
    if (state_changed)
        _spinning = !_spinning;
    auto speed = (_spinning) ? 180 : 0;
    _blade.write(speed);
}

void Blade::shutdown()
{
    _spinning = false;
    _blade.write(0);
}
