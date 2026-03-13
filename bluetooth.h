#pragma once

#include <arduino-timer.h>

#include "constants.h"
#include "packet.h"

namespace bt
{
    void bluetooth_connection_changed();
    bool check_packets_received(void*);


    class Bluetooth {
    private:
        Timer<> _connection_timer;

        void (*_shutdown) ();

    public:
        Bluetooth() {}

        void begin(void (*shutdown)());

        bool is_connected();
        void check_connection();
        void wait_packet_available();
        RadioPacket read_packet();
    };
}
