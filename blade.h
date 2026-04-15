#pragma once

#include <Arduino.h>
#include <Servo.h>

class Blade
{
    private:
        bool _spinning;
        uint16_t _speed;
        Servo _blade;

    public:
        Blade() {}

        void begin(int pin);
        void shutdown();
        void handle(bool state_changed);
};
