#include "packet.h"

int16_t calculate_checksum(const RadioPacket& packet)
{
    auto packet_copy = packet;
    packet_copy.checksum = 0;
    int16_t result = 0;

    int16_t* bytes = reinterpret_cast<int16_t*>(&packet_copy);
    for (auto i = 0; i < sizeof(RadioPacket); i += sizeof(int16_t))
        result ^= *(bytes++);

    return result;
}
