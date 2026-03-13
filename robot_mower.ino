#include <Arduino.h>

#include "blade.h"
#include "bluetooth.h"
#include "constants.h"
#include "packet.h"
#include "wheel.h"


bt::Bluetooth bluetooth;
Wheel left_wheel, right_wheel;
Blade blade;

void shutdown();

void setup()
{
    left_wheel.begin(LEFT_WHEEL_PIN);
    right_wheel.begin(RIGHT_WHEEL_PIN);
    blade.begin(BLADE_PIN);
    bluetooth.begin(shutdown);
}

void shutdown()
{
    blade.shutdown();
    left_wheel.shutdown();
    right_wheel.shutdown();
}

void handle_packet(const RadioPacket& packet)
{
    left_wheel.set_speed(packet.left_y);
    right_wheel.set_speed(packet.right_y);
    blade.handle(packet.right_clicked);
}

void loop()
{
    auto packet = bluetooth.read_packet();
    handle_packet(packet);
}
