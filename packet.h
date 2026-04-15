#pragma once

#include <Arduino.h>

static constexpr int16_t PACKET_HEADER = 0xaa55;

struct __attribute__((packed)) RadioPacket
{
    int16_t header = PACKET_HEADER;
    int16_t left_x;
    int16_t left_y;
    uint8_t left_clicked;

    int16_t right_x;
    int16_t right_y;
    uint8_t right_clicked;

    int16_t checksum;
};

int16_t calculate_checksum(const RadioPacket& packet);

