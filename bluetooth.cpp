#include <Arduino.h>

#include "bluetooth.h"
#include "constants.h"

namespace bt
{
    volatile bool _connected = false;
    volatile bool _packets_received = false;
    volatile int _last_packet_checked = 0u;
    int _last_packet_received = 1u;

    void bluetooth_connection_changed()
    {
        _connected = digitalRead(BLUETOOTH_STATE_PIN);
    }


    bool check_packets_received(void*)
    {
        _packets_received = _last_packet_checked != _last_packet_received;
        _last_packet_checked = _last_packet_received;
        return true;
    }

    void Bluetooth::begin(void (*shutdown)())
    {
        pinMode(BLUETOOTH_STATE_PIN, INPUT);
        attachInterrupt(digitalPinToInterrupt(BLUETOOTH_STATE_PIN), bt::bluetooth_connection_changed, CHANGE);

        _shutdown = shutdown;

        _connection_timer = timer_create_default();
        _connection_timer.every(500, check_packets_received);

        Serial.begin(115200);
        while (!Serial || !_connected) {}
    }

    bool Bluetooth::is_connected()
    {
        return _connected && (_last_packet_checked != _last_packet_received);
    }

    void Bluetooth::check_connection()
    {
        _connection_timer.tick();
        if (!is_connected())
            _shutdown();
    }

    void Bluetooth::wait_packet_available()
    {
        auto available = false;
        while (!available){
            check_connection();
            for (auto byte_read = Serial.read(); byte_read != 0xaa; byte_read = Serial.read())
                if (byte_read == -1)
                    check_connection();
            while (!Serial.available())
                check_connection();
            if (Serial.read() != 0x55)
                continue;
            while (Serial.available() < sizeof(RadioPacket))
                check_connection();
            available = true;
        }
        _last_packet_received++;
    }

    RadioPacket Bluetooth::read_packet()
    {
        wait_packet_available();
        RadioPacket packet;
        Serial.readBytes(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
        return packet;
    }
}
