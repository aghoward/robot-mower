#include <Arduino.h>
#include <arduino-timer.h>
#include <SPI.h>
#include <Servo.h>

#include "packet.h"

#define BLUETOOTH_STATE_PIN   3

#define BLADE_PIN             11
#define LEFT_WHEEL_PIN        10
#define RIGHT_WHEEL_PIN        9
#define WHEEL_OFF_ANGLE       90

#define bt Serial

Servo left_wheel, right_wheel, blade;
bool blade_spinning = false;

Timer connection_timer;

volatile bool bluetooth_connected = false;
volatile bool receiving_packets = false;
volatile int last_packet_received = 0;
volatile int current_packet_received = 1;

void shutdown();

void bluetooth_connection_changed()
{
    bluetooth_connected = digitalRead(BLUETOOTH_STATE_PIN);
}

bool check_packets_received(void*)
{
    receiving_packets = last_packet_received != current_packet_received;
    last_packet_received = current_packet_received;
    return true;
}

bool is_connected()
{
    return bluetooth_connected && receiving_packets;
}

void check_connection()
{
    connection_timer.tick();
    if (!is_connected())
        shutdown();
}

void wait_packet_available()
{
    auto available = false;
    while (!available){
        check_connection();
        for (auto byte_read = bt.read(); byte_read != 0xaa; byte_read = bt.read())
            if (byte_read == -1)
                check_connection();
        while (!bt.available())
            check_connection();
        if (bt.read() != 0x55)
            continue;
        while (bt.available() < sizeof(RadioPacket))
            check_connection();
        available = true;
    }
    current_packet_received++;
}

RadioPacket read_packet()
{
    wait_packet_available();
    RadioPacket packet;
    bt.readBytes(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
    return packet;
}

void setup()
{

    pinMode(BLUETOOTH_STATE_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(BLUETOOTH_STATE_PIN), &bluetooth_connection_changed, CHANGE);

    analogWrite(LEFT_WHEEL_PIN, WHEEL_OFF_ANGLE);
    analogWrite(RIGHT_WHEEL_PIN, WHEEL_OFF_ANGLE);

    left_wheel.attach(LEFT_WHEEL_PIN, 1000, 2000);
    right_wheel.attach(RIGHT_WHEEL_PIN, 1000, 2000);
    blade.attach(BLADE_PIN, 1000, 2000);
    blade.write(0);

    connection_timer = timer_create_default();
    connection_timer.every(500, check_packets_received);

    bt.begin(115200);
    while (!bt || !bluetooth_connected) {}
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
    auto packet = read_packet();
    handle_packet(packet);
}
