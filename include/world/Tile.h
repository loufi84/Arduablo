#pragma once

#include <stdint.h>

enum class Tile : uint8_t {
    FLOOR = 0,
    WALL,
    STAIRS_DOWN
};
