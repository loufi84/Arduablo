#include <Arduboy2.h>

#include "rendering/Renderer.h"
#include "world/Dungeon.h"
#include "entities/Player.h"
#include "entities/Monster.h"

extern Arduboy2 arduboy;

constexpr uint8_t TILE_SIZE = 8;

void Renderer::drawDungeon(const Dungeon& dungeon) {
    for (uint8_t y = 0; y < Dungeon::MAP_HEIGHT; ++y) {
        for (uint8_t x = 0; x < Dungeon::MAP_WIDTH; ++x) {

            const int16_t px = x * TILE_SIZE;
            const int16_t py = y * TILE_SIZE;

            if (dungeon.getTile(x, y) == Tile::WALL) {
                arduboy.fillRect(px, py, TILE_SIZE, TILE_SIZE, WHITE);
            }
        }
    }
}

void Renderer::drawPlayer(const Player& player) {
    const Position pos = player.getPosition();

    arduboy.fillRect(
        pos.x * TILE_SIZE + 2,
        pos.y * TILE_SIZE + 2,
        4,
        4,
        WHITE
    );
}

void Renderer::drawMonsters(const Monster* monsters, uint8_t monsterCount) {
    for (uint8_t i = 0; i < monsterCount; ++i) {
        if (!monsters[i].isAlive()) {
            continue;
        }

        const Position pos = monsters[i].getPosition();

        const int16_t px = pos.x * TILE_SIZE;
        const int16_t py = pos.y * TILE_SIZE;

        arduboy.drawRect(
            px + 1,
            py + 1,
            6,
            6,
            WHITE
        );

        arduboy.drawPixel(
            px + 2,
            py + 3,
            WHITE
        );

        arduboy.drawPixel(
            px + 5,
            py + 3,
            WHITE
        );
    }
}
