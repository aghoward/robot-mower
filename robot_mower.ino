#include <Arduino.h>
#include <Servo.h>

#include "bluetooth.h"
#include "constants.h"
#include "packet.h"
#include "wheel.h"


bt::Bluetooth* bluetooth;
Wheel* left_wheel, * right_wheel;
Servo blade;
bool blade_spinning = false;

void shutdown();

void setup()
{
    blade.attach(BLADE_PIN, 1000, 2000);
    blade.write(0);
    left_wheel = new Wheel(LEFT_WHEEL_PIN);
    right_wheel = new Wheel(RIGHT_WHEEL_PIN);
    bluetooth = new bt::Bluetooth(shutdown);
}

void shutdown()
{
    blade_spinning = false;
    blade.write(0);
    left_wheel->shutdown();
    right_wheel->shutdown();
}

void handle_blade(bool state_changed)
{
    static auto blade_speed = 0u;

    if (state_changed)
        blade_spinning = !blade_spinning;
    if (blade_spinning && blade_speed < 180)
    {
        if (blade_speed == 0u)
            blade_speed = 10u;
        else
            blade_speed++;
    }
    else if (!blade_spinning)
        blade_speed = 0u;
    blade.write(blade_speed);
}
}

void handle_packet(const RadioPacket& packet)
{
    left_wheel->set_speed(packet.left_y);
    right_wheel->set_speed(packet.right_y);

    handle_blade(packet.right_clicked);
}

void loop()
{
    auto packet = bluetooth->read_packet();
    handle_packet(packet);
}
