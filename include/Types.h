#pragma once

#include <stdint.h>

enum class Direction : uint8_t {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Position {
    uint8_t x;
    uint8_t y;
};
