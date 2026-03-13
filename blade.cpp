#include "blade.h"

void Blade::begin(int pin)
{
    _spinning = false;
    _blade = Servo();
    _blade.attach(pin, 1000, 2000);
    _blade.write(0);
}

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
