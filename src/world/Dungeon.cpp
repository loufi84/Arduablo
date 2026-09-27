#include "world/Dungeon.h"

void Dungeon::begin() {
}

void Dungeon::update() {
}

Tile Dungeon::getTile(uint8_t x, uint8_t y) const {
    // Important aussi pour nous protéger d'un x/y invalide
    if (x >= MAP_WIDTH || y >= MAP_HEIGHT) {
        return Tile::WALL;
    }

    const uint16_t index =
        static_cast<uint16_t>(y) * MAP_WIDTH + x;

    const uint16_t byteIndex = index >> 1;

    uint8_t value;

    if (index & 1) {
        // Tile impaire -> 4 bits de poids fort
        value = tiles[byteIndex] >> 4;
    }
    else {
        // Tile paire -> 4 bits de poids faible
        value = tiles[byteIndex] & 0x0F;
    }

    return static_cast<Tile>(value);
}

void Dungeon::setTile(
    uint8_t x,
    uint8_t y,
    Tile tile
) {
    if (x >= MAP_WIDTH || y >= MAP_HEIGHT) {
        return;
    }

    const uint16_t index =
        static_cast<uint16_t>(y) * MAP_WIDTH + x;

    const uint16_t byteIndex = index >> 1;

    const uint8_t value =
        static_cast<uint8_t>(tile) & 0x0F;

    if (index & 1) {
        // On conserve le nibble bas
        tiles[byteIndex] =
            (tiles[byteIndex] & 0x0F)
            | (value << 4);
    }
    else {
        // On conserve le nibble haut
        tiles[byteIndex] =
            (tiles[byteIndex] & 0xF0)
            | value;
    }
}

bool Dungeon::isWalkable(uint8_t x, uint8_t y) const {
    const Tile tile = getTile(x, y);

    return tile == Tile::FLOOR
        || tile == Tile::STAIRS_DOWN;
}

Position Dungeon::getStartPosition() const {
    return startPosition;
}

Position Dungeon::getExitPosition() const {
    return exitPosition;
}

void Dungeon::setStartPosition(Position position) {
    startPosition = position;
}

void Dungeon::setExitPosition(Position position) {
    exitPosition = position;
}
