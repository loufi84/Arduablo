#pragma once

#include <stdint.h>
#include "world/Tile.h"

class Dungeon {
    public:
        static constexpr uint8_t MAP_WIDTH = 16;
        static constexpr uint8_t MAP_HEIGHT = 7;

        void begin();
        void update();

        Tile getTile(uint8_t x, uint8_t y) const;
        bool isWalkable(uint8_t x, uint8_t y) const;

    private:
        Tile tiles[MAP_WIDTH * MAP_HEIGHT];
};
