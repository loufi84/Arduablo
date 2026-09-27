#include "world/Dungeon.h"

void Dungeon::begin() {
    for (uint8_t y = 0; y < MAP_HEIGHT; ++y) {
        for (uint8_t x = 0; x < MAP_WIDTH; ++x) {
            const bool border =
                x == 0 ||
                y == 0 ||
                x == MAP_WIDTH - 1 ||
                y == MAP_HEIGHT - 1;

            tiles[y * MAP_WIDTH * x] =
                border ? Tile::WALL : Tile::FLOOR;
        }
    }
}

void Dungeon::update() {

}

Tile Dungeon::getTile(uint8_t x, uint8_t y) const {
    return tiles[y * MAP_WIDTH * x];
}

bool Dungeon::isWalkable(uint8_t x, uint8_t y) const {
    return getTile(x, y) == Tile::FLOOR;
}
