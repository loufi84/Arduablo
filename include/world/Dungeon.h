#pragma once

#include <stdint.h>

#include "Config.h"
#include "Types.h"
#include "world/Tile.h"

class Dungeon {
public:
    static constexpr uint8_t MAP_WIDTH = ::MAP_WIDTH;
    static constexpr uint8_t MAP_HEIGHT = ::MAP_HEIGHT;

    void begin();
    void update();

    Tile getTile(uint8_t x, uint8_t y) const;
    void setTile(uint8_t x, uint8_t y, Tile tile);

    bool isWalkable(uint8_t x, uint8_t y) const;

    Position getStartPosition() const;
    Position getExitPosition() const;

    void setStartPosition(Position position);
    void setExitPosition(Position position);

private:
    uint8_t tiles[(MAP_WIDTH * MAP_HEIGHT + 1) / 2];

    Position startPosition { 1,1 };
    Position exitPosition { 1,1 };
};
