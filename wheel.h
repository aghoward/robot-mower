#pragma once

#include <Arduino.h>
#include <Servo.h>

class Wheel
{
    private:
        static constexpr int OFF_ANGLE = 90;
        static constexpr int DEAD_ZONE_THRESHOLD = 10;

        Servo _wheel;
        int _pin;

        int get_motor_speed(int16_t stick_value);

    public:
        Wheel() {}

        void begin(int pin);
        void shutdown();
        void set_speed(int16_t speed);
};
