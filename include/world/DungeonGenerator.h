#pragma once

#include <stdint.h>

#include "Types.h"
#include "world/Room.h"

class Dungeon;

class DungeonGenerator {
    public:
        static void generate(Dungeon& dungeon);

        static bool findRandomFloor(const Dungeon& dungeon, Position& result);

    private:
        static bool intersects(const Room& a, const Room& b);
        static void carveRoom(Dungeon& dungeon, const Room& room);
        static void carveCorridor(Dungeon& dungeon, Position from, Position to);
        static void carveHorizontal(Dungeon& dungeon, int8_t x1, int8_t x2, int8_t y);
        static void carveVertical(Dungeon& dungeon, int8_t y1, int8_t y2, int8_t x);
};
