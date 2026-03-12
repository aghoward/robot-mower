#pragma once

struct __attribute__((packed)) RadioPacket
{
    int16_t left_x;
    int16_t left_y;
    uint8_t left_clicked;

    int16_t right_x;
    int16_t right_y;
    uint8_t right_clicked;
};
