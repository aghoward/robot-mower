#include <Arduino.h>
#include <arduino-timer.h>
#include <SPI.h>
#include <Servo.h>

#include "bluetooth.h"
#include "constants.h"
#include "packet.h"


bt::Bluetooth* bluetooth;
Servo left_wheel, right_wheel, blade;
bool blade_spinning = false;

void shutdown();

void setup()
{
    analogWrite(LEFT_WHEEL_PIN, WHEEL_OFF_ANGLE);
    analogWrite(RIGHT_WHEEL_PIN, WHEEL_OFF_ANGLE);

    left_wheel.attach(LEFT_WHEEL_PIN, 1000, 2000);
    right_wheel.attach(RIGHT_WHEEL_PIN, 1000, 2000);
    blade.attach(BLADE_PIN, 1000, 2000);
    blade.write(0);

    bluetooth = new bt::Bluetooth(shutdown);
}

void shutdown()
{
    blade_spinning = false;
    blade.write(0);
    left_wheel.write(WHEEL_OFF_ANGLE);
    right_wheel.write(WHEEL_OFF_ANGLE);
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

int get_motor_speed(int16_t stick_value)
{
    const auto dead_zone_threshold = 10;
    auto value = map(stick_value, -255, 255, 0, 180);
    if (value > (WHEEL_OFF_ANGLE - dead_zone_threshold)
        && value < (WHEEL_OFF_ANGLE + dead_zone_threshold))
        return WHEEL_OFF_ANGLE;
    return value;
}

void handle_packet(const RadioPacket& packet)
{
    auto left = get_motor_speed(packet.left_y);
    auto right = get_motor_speed(packet.right_y);
    left_wheel.write(left);
    right_wheel.write(right);

    handle_blade(packet.right_clicked);
}

void loop()
{
    auto packet = bluetooth->read_packet();
    handle_packet(packet);
}
