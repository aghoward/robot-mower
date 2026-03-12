#include <Arduino.h>

#include "wheel.h"

void Wheel::shutdown()
{
    _wheel.write(OFF_ANGLE);
}

int Wheel::get_motor_speed(int16_t stick_value)
{
    auto value = map(stick_value, -255, 255, 0, 180);
    if (value > (OFF_ANGLE - DEAD_ZONE_THRESHOLD)
        && value < (OFF_ANGLE + DEAD_ZONE_THRESHOLD))
        return OFF_ANGLE;
    return value;
}

void Wheel::set_speed(int16_t speed)
{
    _wheel.write(get_motor_speed(speed));
}
