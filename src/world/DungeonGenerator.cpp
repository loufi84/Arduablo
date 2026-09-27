#include <Arduino.h>

#include "Config.h"
#include "world/Dungeon.h"
#include "world/DungeonGenerator.h"

void DungeonGenerator::generate(Dungeon& dungeon) {
    // On commence par un énorme bloc de murs
    for (uint8_t y = 0; y < MAP_HEIGHT; ++y) {
        for (uint8_t x = 0; x < MAP_WIDTH; ++x) {
            dungeon.setTile(x, y, Tile::WALL);
        }
    }

    Room rooms[MAX_ROOMS];
    uint8_t roomCount = 0;

    const uint8_t targetRoomCount =
        random(MIN_ROOMS, MAX_ROOMS + 1);

    for (
        uint8_t attempt = 0;
        attempt < ROOM_GENERATION_ATTEMPTS
        && roomCount < targetRoomCount;
        ++attempt
    ) {
        const uint8_t width =
            random(MIN_ROOM_SIZE, MAX_ROOM_SIZE + 1);

        const uint8_t height =
            random(MIN_ROOM_SIZE, MAX_ROOM_SIZE + 1);

        const uint8_t x =
            random(1, MAP_WIDTH - width - 1);

        const uint8_t y =
            random(1, MAP_HEIGHT - height - 1);

        Room candidate {
            x,
            y,
            width,
            height
        };

        bool overlaps = false;

        for (uint8_t i = 0; i < roomCount; ++i) {
            if (intersects(candidate, rooms[i])) {
                overlaps = true;
                break;
            }
        }

        if (overlaps) {
            continue;
        }

        carveRoom(dungeon, candidate);

        if (roomCount > 0) {
            carveCorridor(
                dungeon,
                rooms[roomCount - 1].center(),
                candidate.center()
            );
        }

        rooms[roomCount] = candidate;
        ++roomCount;
    }

    if (roomCount == 0) {
        return;
    }

    const Position start =
        rooms[0].center();

    const Position exit =
        rooms[roomCount - 1].center();

    dungeon.setStartPosition(start);
    dungeon.setExitPosition(exit);

    dungeon.setTile(
        exit.x,
        exit.y,
        Tile::STAIRS_DOWN
    );
}

bool DungeonGenerator::intersects(
    const Room& a,
    const Room& b
) {
    const int16_t aLeft   = a.x - 1;
    const int16_t aRight  = a.x + a.width;
    const int16_t aTop    = a.y - 1;
    const int16_t aBottom = a.y + a.height;

    const int16_t bLeft   = b.x - 1;
    const int16_t bRight  = b.x + b.width;
    const int16_t bTop    = b.y - 1;
    const int16_t bBottom = b.y + b.height;

    return !(
        aRight < bLeft ||
        aLeft > bRight ||
        aBottom < bTop ||
        aTop > bBottom
    );
}

void DungeonGenerator::carveRoom(
    Dungeon& dungeon,
    const Room& room
) {
    for (
        uint8_t y = room.y;
        y < room.y + room.height;
        ++y
    ) {
        for (
            uint8_t x = room.x;
            x < room.x + room.width;
            ++x
        ) {
            dungeon.setTile(
                x,
                y,
                Tile::FLOOR
            );
        }
    }
}

void DungeonGenerator::carveCorridor(
    Dungeon& dungeon,
    Position from,
    Position to
) {
    if (random(0, 2) == 0) {
        carveHorizontal(
            dungeon,
            from.x,
            to.x,
            from.y
        );

        carveVertical(
            dungeon,
            from.y,
            to.y,
            to.x
        );
    }
    else {
        carveVertical(
            dungeon,
            from.y,
            to.y,
            from.x
        );

        carveHorizontal(
            dungeon,
            from.x,
            to.x,
            to.y
        );
    }
}

void DungeonGenerator::carveHorizontal(
    Dungeon& dungeon,
    int8_t x1,
    int8_t x2,
    int8_t y
) {
    if (x1 > x2) {
        const int8_t tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    for (int8_t x = x1; x <= x2; ++x) {
        dungeon.setTile(
            x,
            y,
            Tile::FLOOR
        );
    }
}

void DungeonGenerator::carveVertical(
    Dungeon& dungeon,
    int8_t y1,
    int8_t y2,
    int8_t x
) {
    if (y1 > y2) {
        const int8_t tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    for (int8_t y = y1; y <= y2; ++y) {
        dungeon.setTile(
            x,
            y,
            Tile::FLOOR
        );
    }
}

bool DungeonGenerator::findRandomFloor(
    const Dungeon& dungeon,
    Position& result
) {
    for (uint8_t attempt = 0; attempt < 50; ++attempt) {
        const uint8_t x =
            random(1, MAP_WIDTH - 1);

        const uint8_t y =
            random(1, MAP_HEIGHT - 1);

        if (dungeon.getTile(x, y) == Tile::FLOOR) {
            result = {
                static_cast<int8_t>(x),
                static_cast<int8_t>(y)
            };

            return true;
        }
    }

    return false;
}
