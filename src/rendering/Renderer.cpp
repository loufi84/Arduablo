#include <Arduboy2.h>

#include "Config.h"
#include "rendering/Renderer.h"
#include "rendering/Camera.h"
#include "data/Sprites.h"

#include "world/Dungeon.h"
#include "entities/Player.h"
#include "entities/Monster.h"
#include "items/GroundItem.h"

extern Arduboy2 arduboy;

void Renderer::drawDungeon(const Dungeon& dungeon, const Camera& camera) {
    for (
        uint8_t screenY = 0;
        screenY < VIEW_HEIGHT;
        ++screenY
    ) {
        for (
            uint8_t screenX = 0;
            screenX < VIEW_WIDTH;
            ++screenX
        ) {
            const uint8_t worldX = camera.getX() + screenX;
            const uint8_t worldY = camera.getY() + screenY;

            const Tile tile =
                dungeon.getTile(worldX, worldY);

            const int16_t px =
                screenX * TILE_SIZE;

            const int16_t py =
                screenY * TILE_SIZE;

            if (tile == Tile::WALL) {
                arduboy.fillRect(
                    px,
                    py,
                    TILE_SIZE,
                    TILE_SIZE,
                    WHITE
                );
            }
            else if (tile == Tile::STAIRS_DOWN) {
                arduboy.drawLine(
                    px + 1,
                    py + 2,
                    px + 6,
                    py + 2,
                    WHITE
                );

                arduboy.drawLine(
                    px + 2,
                    py + 4,
                    px + 6,
                    py + 4,
                    WHITE
                );

                arduboy.drawLine(
                    px + 3,
                    py + 6,
                    px + 6,
                    py + 6,
                    WHITE
                );
            }
        }
    }
}

void Renderer::drawPlayer(const Player& player, const Camera& camera) {
    const Position pos =
        player.getPosition();

    const int16_t screenTileX =
        static_cast<int16_t>(pos.x) - camera.getX();

    const int16_t screenTileY =
        static_cast<int16_t>(pos.y) - camera.getY();

    const int16_t px = screenTileX * TILE_SIZE;
    const int16_t py = screenTileY * TILE_SIZE;

    Sprites::drawSelfMasked(px, py, GameSprites::PLAYER, 0);
}

void Renderer::drawMonsters(const Monster* monsters, uint8_t monsterCount, const Camera& camera) {
    for (
        uint8_t i = 0;
        i < monsterCount;
        ++i
    ) {
        if (!monsters[i].isAlive()) {
            continue;
        }

        const Position pos =
            monsters[i].getPosition();

        const int16_t screenTileX =
            static_cast<int16_t>(pos.x)
            - camera.getX();

        const int16_t screenTileY =
            static_cast<int16_t>(pos.y)
            - camera.getY();

        // Le monstre est hors écran
        if (
            screenTileX < 0 ||
            screenTileX >= VIEW_WIDTH ||
            screenTileY < 0 ||
            screenTileY >= VIEW_HEIGHT
        ) {
            continue;
        }

        const int16_t px =
            screenTileX * TILE_SIZE;

        const int16_t py =
            screenTileY * TILE_SIZE;

        Sprites::drawSelfMasked(px, py, GameSprites::ZOMBIE, 0);
    }
}

void Renderer::drawPlayerAttack(
    const Player& player,
    const Camera& camera
) {
    if (!player.isAttacking()) {
        return;
    }

    const Position pos = player.getPosition();

    const int16_t px =
        (static_cast<int16_t>(pos.x) - camera.getX())
        * TILE_SIZE;

    const int16_t py =
        (static_cast<int16_t>(pos.y) - camera.getY())
        * TILE_SIZE;

    switch (player.getDirection()) {

        case Direction::RIGHT:
            // garde
            arduboy.drawLine(
                px + 5, py + 2,
                px + 5, py + 6,
                WHITE
            );

            // lame
            arduboy.drawLine(
                px + 6, py + 4,
                px + 12, py + 4,
                WHITE
            );

            arduboy.drawPixel(px + 13, py + 4, WHITE);
            break;

        case Direction::LEFT:
            arduboy.drawLine(
                px + 2, py + 2,
                px + 2, py + 6,
                WHITE
            );

            arduboy.drawLine(
                px + 1, py + 4,
                px - 5, py + 4,
                WHITE
            );

            arduboy.drawPixel(px - 6, py + 4, WHITE);
            break;

        case Direction::UP:
            arduboy.drawLine(
                px + 2, py + 2,
                px + 6, py + 2,
                WHITE
            );

            arduboy.drawLine(
                px + 4, py + 1,
                px + 4, py - 5,
                WHITE
            );

            arduboy.drawPixel(px + 4, py - 6, WHITE);
            break;

        case Direction::DOWN:
            arduboy.drawLine(
                px + 2, py + 5,
                px + 6, py + 5,
                WHITE
            );

            arduboy.drawLine(
                px + 4, py + 6,
                px + 4, py + 12,
                WHITE
            );

            arduboy.drawPixel(px + 4, py + 13, WHITE);
            break;
    }
}

void Renderer::drawMonsterAttack(
    const Monster* monsters,
    uint8_t monsterCount,
    const Camera& camera
) {
    for (uint8_t i = 0; i < monsterCount; ++i) {

        if (
            !monsters[i].isAlive() ||
            !monsters[i].isAttacking()
        ) {
            continue;
        }

        const Position pos =
            monsters[i].getPosition();

        const int16_t screenX =
            static_cast<int16_t>(pos.x)
            - camera.getX();

        const int16_t screenY =
            static_cast<int16_t>(pos.y)
            - camera.getY();

        if (
            screenX < 0 ||
            screenX >= VIEW_WIDTH ||
            screenY < 0 ||
            screenY >= VIEW_HEIGHT
        ) {
            continue;
        }

        const int16_t px = screenX * TILE_SIZE;
        const int16_t py = screenY * TILE_SIZE;

        switch (monsters[i].getAttackDirection()) {

            case Direction::RIGHT:
                arduboy.drawLine(
                    px + 4, py + 4,
                    px + 9, py + 4,
                    WHITE
                );

                // poing
                arduboy.fillRect(
                    px + 9,
                    py + 3,
                    2,
                    3,
                    WHITE
                );
                break;

            case Direction::LEFT:
                arduboy.drawLine(
                    px + 3, py + 4,
                    px - 2, py + 4,
                    WHITE
                );

                arduboy.fillRect(
                    px - 4,
                    py + 3,
                    2,
                    3,
                    WHITE
                );
                break;

            case Direction::UP:
                arduboy.drawLine(
                    px + 4, py + 3,
                    px + 4, py - 2,
                    WHITE
                );

                arduboy.fillRect(
                    px + 3,
                    py - 4,
                    3,
                    2,
                    WHITE
                );
                break;

            case Direction::DOWN:
                arduboy.drawLine(
                    px + 4, py + 4,
                    px + 4, py + 9,
                    WHITE
                );

                arduboy.fillRect(
                    px + 3,
                    py + 9,
                    3,
                    2,
                    WHITE
                );
                break;
        }
    }
}

void Renderer::drawGroundItem(
    const GroundItem* items,
    uint8_t itemCount,
    const Camera& camera
) {
    for (uint8_t i = 0; i < itemCount; ++i) {
        if (!items[i].active) {
            continue;
        }

        const Position pos =
            items[i].position;

        const int16_t screenX =
            static_cast<int16_t>(pos.x)
            - camera.getX();

        const int16_t screenY =
            static_cast<int16_t>(pos.y)
            - camera.getY();

        if (
            screenX < 0 ||
            screenX >= VIEW_WIDTH ||
            screenY < 0 ||
            screenY >= VIEW_HEIGHT
        ) {
            continue;
        }

        const int16_t px =
            screenX * TILE_SIZE;

        const int16_t py =
            screenY * TILE_SIZE;

        // Mini épée temporaire
        arduboy.drawLine(
            px + 2,
            py + 6,
            px + 6,
            py + 2,
            WHITE
        );

        arduboy.drawLine(
            px + 2,
            py + 4,
            px + 4,
            py + 6,
            WHITE
        );
    }
}
