#pragma once

#include <stdint.h>
#include "Types.h"

struct Room {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;

    Position center() const {
        return {
            static_cast<int8_t>(x + width / 2),
            static_cast<int8_t>(y + height / 2)
        };
    }
};
