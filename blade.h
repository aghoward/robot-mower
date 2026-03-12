#pragma once

#include <Arduino.h>
#include <Servo.h>

class Blade
{
    private:
        bool _spinning;
        Servo _blade;

    public:
        Blade(int pin)
            : _spinning(false),
              _blade()
        {
            _blade.attach(pin, 1000, 2000);
            _blade.write(0);
        }

        void shutdown();
        void handle(bool state_changed);
};
