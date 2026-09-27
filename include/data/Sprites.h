#pragma once

#include <stdint.h>
#include <avr/pgmspace.h>

namespace GameSprites {

    constexpr uint8_t PLAYER[] PROGMEM = {
        8, 8,

        0x00,
        0x8E,
        0xEB,
        0x7F,
        0x7F,
        0xEB,
        0x8E,
        0x00
    };

    constexpr uint8_t ZOMBIE[] PROGMEM = {
        8, 8,

        0x00,
        0xAE,
        0x5B,
        0x3F,
        0x3F,
        0x5B,
        0xAE,
        0x00
    };
}
