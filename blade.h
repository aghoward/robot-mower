#pragma once

#include <Arduino.h>
#include <Servo.h>

class Blade
{
    private:
        bool _spinning;
        Servo _blade;

    public:
        Blade() {}

        void begin(int pin);
        void shutdown();
        void handle(bool state_changed);
};
