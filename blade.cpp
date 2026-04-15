#include "blade.h"

void Blade::begin(int pin)
{
    _spinning = false;
    _speed = 0u;
    _blade = Servo();
    _blade.attach(pin, 700, 2200);
    _blade.write(0);
}

void Blade::handle(bool state_changed)
{
    if (state_changed)
        _spinning = !_spinning;
    if (_spinning && _speed < 180)
        _speed++;
    else if (!_spinning)
        _speed = 0u;
    _blade.write(_speed);
}

void Blade::shutdown()
{
    _spinning = false;
    _blade.write(0);
}
