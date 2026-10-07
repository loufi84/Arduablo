#include <Arduboy2.h>

#include "Config.h"

#include "rendering/Renderer.h"
#include "rendering/Camera.h"

#include "world/Dungeon.h"
#include "world/Tile.h"

#include "entities/Player.h"
#include "entities/Monster.h"

#include "items/GroundItem.h"
#include "items/Item.h"

#include "data/Sprites.h"
#include "data/MonsterData.h"


extern Arduboy2 arduboy;


namespace {

constexpr int16_t TILE_HALF =
    TILE_SIZE / 2;

constexpr int16_t TILE_QUARTER =
    TILE_SIZE / 4;


// ============================================================
// Conversion coordonnées monde -> écran
// ============================================================

bool worldToScreen(
    Position position,
    const Camera& camera,
    int16_t& px,
    int16_t& py
) {
    const int16_t screenX =
        static_cast<int16_t>(position.x)
        -
        camera.getX();

    const int16_t screenY =
        static_cast<int16_t>(position.y)
        -
        camera.getY();


    if (
        screenX < 0
        ||
        screenY < 0
        ||
        screenX >= VIEW_WIDTH
        ||
        screenY >= VIEW_HEIGHT
    ) {
        return false;
    }


    px =
        screenX * TILE_SIZE;

    py =
        screenY * TILE_SIZE;


    return true;
}

} // namespace


// ============================================================
// Donjon
// ============================================================

void Renderer::drawDungeon(
    const Dungeon& dungeon,
    const Camera& camera
) {
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
            const uint8_t worldX =
                camera.getX()
                +
                screenX;

            const uint8_t worldY =
                camera.getY()
                +
                screenY;


            const Tile tile =
                dungeon.getTile(
                    worldX,
                    worldY
                );


            const int16_t px =
                screenX * TILE_SIZE;

            const int16_t py =
                screenY * TILE_SIZE;


            switch (tile) {

                case Tile::FLOOR:

                    Sprites::drawSelfMasked(
                        px,
                        py,
                        GameSprites::FLOOR,
                        0
                    );

                    break;


                case Tile::WALL:

                    Sprites::drawSelfMasked(
                        px,
                        py,
                        GameSprites::WALL,
                        0
                    );

                    break;


                case Tile::STAIRS_DOWN:

                    Sprites::drawSelfMasked(
                        px,
                        py,
                        GameSprites::STAIRS_DOWN,
                        0
                    );

                    break;
            }
        }
    }
}


// ============================================================
// Joueur
// ============================================================

void Renderer::drawPlayer(
    const Player& player,
    const Camera& camera
) {
    int16_t px;
    int16_t py;


    if (
        !worldToScreen(
            player.getPosition(),
            camera,
            px,
            py
        )
    ) {
        return;
    }


    Sprites::drawSelfMasked(
        px,
        py,
        GameSprites::PLAYER,
        0
    );
}


// ============================================================
// Monstres
// ============================================================

void Renderer::drawMonsters(
    const Monster* monsters,
    uint8_t monsterCount,
    const Camera& camera
) {
    for (
        uint8_t i = 0;
        i < monsterCount;
        ++i
    ) {
        if (!monsters[i].isAlive()) {
            continue;
        }


        int16_t px;
        int16_t py;


        if (
            !worldToScreen(
                monsters[i].getPosition(),
                camera,
                px,
                py
            )
        ) {
            continue;
        }


        switch (
            monsters[i].getType()
        ) {

            case MonsterType::ZOMBIE:

                Sprites::drawSelfMasked(
                    px,
                    py,
                    GameSprites::ZOMBIE,
                    0
                );

                break;


            case MonsterType::SKELETON:

                Sprites::drawSelfMasked(
                    px,
                    py,
                    GameSprites::SKELETON,
                    0
                );

                break;


            case MonsterType::BRUTE:

                Sprites::drawSelfMasked(
                    px,
                    py,
                    GameSprites::BRUTE,
                    0
                );

                break;
        }
    }
}


// ============================================================
// Objets au sol
// ============================================================

void Renderer::drawGroundItem(
    const GroundItem* items,
    uint8_t itemCount,
    const Camera& camera
) {
    for (
        uint8_t i = 0;
        i < itemCount;
        ++i
    ) {
        if (!items[i].active) {
            continue;
        }


        int16_t px;
        int16_t py;


        if (
            !worldToScreen(
                items[i].position,
                camera,
                px,
                py
            )
        ) {
            continue;
        }


        switch (
            items[i].item.type
        ) {

            case ItemType::DAGGER:

                Sprites::drawSelfMasked(
                    px,
                    py,
                    GameSprites::DAGGER,
                    0
                );

                break;


            case ItemType::SWORD:

                Sprites::drawSelfMasked(
                    px,
                    py,
                    GameSprites::SWORD,
                    0
                );

                break;


            case ItemType::AXE:

                Sprites::drawSelfMasked(
                    px,
                    py,
                    GameSprites::AXE,
                    0
                );

                break;


            default:
                break;
        }
    }
}


// ============================================================
// Attaque joueur
// ============================================================

void Renderer::drawPlayerAttack(
    const Player& player,
    const Camera& camera
) {
    if (!player.isAttacking()) {
        return;
    }


    int16_t px;
    int16_t py;


    if (
        !worldToScreen(
            player.getPosition(),
            camera,
            px,
            py
        )
    ) {
        return;
    }


    const int16_t centerX =
        px + TILE_HALF;

    const int16_t centerY =
        py + TILE_HALF;


    switch (
        player.getDirection()
    ) {

        case Direction::UP:

            arduboy.drawLine(
                centerX,
                centerY - TILE_QUARTER,
                centerX,
                centerY - TILE_SIZE,
                WHITE
            );

            break;


        case Direction::DOWN:

            arduboy.drawLine(
                centerX,
                centerY + TILE_QUARTER,
                centerX,
                centerY + TILE_SIZE,
                WHITE
            );

            break;


        case Direction::LEFT:

            arduboy.drawLine(
                centerX - TILE_QUARTER,
                centerY,
                centerX - TILE_SIZE,
                centerY,
                WHITE
            );

            break;


        case Direction::RIGHT:

            arduboy.drawLine(
                centerX + TILE_QUARTER,
                centerY,
                centerX + TILE_SIZE,
                centerY,
                WHITE
            );

            break;
    }
}


// ============================================================
// Attaques monstres
// ============================================================

void Renderer::drawMonsterAttack(
    const Monster* monsters,
    uint8_t monsterCount,
    const Camera& camera
) {
    for (
        uint8_t i = 0;
        i < monsterCount;
        ++i
    ) {
        if (
            !monsters[i].isAlive()
            ||
            !monsters[i].isAttacking()
        ) {
            continue;
        }


        int16_t px;
        int16_t py;


        if (
            !worldToScreen(
                monsters[i].getPosition(),
                camera,
                px,
                py
            )
        ) {
            continue;
        }


        const int16_t centerX =
            px + TILE_HALF;

        const int16_t centerY =
            py + TILE_HALF;


        const Direction direction =
            monsters[i].getAttackDirection();


        // ====================================================
        // Skeleton
        // ====================================================

        if (
            monsters[i].getType()
            ==
            MonsterType::SKELETON
        ) {
            switch (direction) {

                case Direction::UP:

                    arduboy.drawLine(
                        centerX,
                        centerY,
                        centerX,
                        centerY - TILE_HALF,
                        WHITE
                    );

                    break;


                case Direction::DOWN:

                    arduboy.drawLine(
                        centerX,
                        centerY,
                        centerX,
                        centerY + TILE_HALF,
                        WHITE
                    );

                    break;


                case Direction::LEFT:

                    arduboy.drawLine(
                        centerX,
                        centerY,
                        centerX - TILE_HALF,
                        centerY,
                        WHITE
                    );

                    break;


                case Direction::RIGHT:

                    arduboy.drawLine(
                        centerX,
                        centerY,
                        centerX + TILE_HALF,
                        centerY,
                        WHITE
                    );

                    break;
            }


            continue;
        }


        // ====================================================
        // Zombie / Brute
        // ====================================================

        const int16_t offset =
            TILE_QUARTER / 2;


        switch (direction) {

            case Direction::UP:

                arduboy.drawLine(
                    centerX - offset,
                    centerY - TILE_QUARTER,
                    centerX - offset,
                    centerY - TILE_HALF,
                    WHITE
                );

                arduboy.drawLine(
                    centerX + offset,
                    centerY - TILE_QUARTER,
                    centerX + offset,
                    centerY - TILE_HALF,
                    WHITE
                );

                break;


            case Direction::DOWN:

                arduboy.drawLine(
                    centerX - offset,
                    centerY + TILE_QUARTER,
                    centerX - offset,
                    centerY + TILE_HALF,
                    WHITE
                );

                arduboy.drawLine(
                    centerX + offset,
                    centerY + TILE_QUARTER,
                    centerX + offset,
                    centerY + TILE_HALF,
                    WHITE
                );

                break;


            case Direction::LEFT:

                arduboy.drawLine(
                    centerX - TILE_QUARTER,
                    centerY - offset,
                    centerX - TILE_HALF,
                    centerY - offset,
                    WHITE
                );

                arduboy.drawLine(
                    centerX - TILE_QUARTER,
                    centerY + offset,
                    centerX - TILE_HALF,
                    centerY + offset,
                    WHITE
                );

                break;


            case Direction::RIGHT:

                arduboy.drawLine(
                    centerX + TILE_QUARTER,
                    centerY - offset,
                    centerX + TILE_HALF,
                    centerY - offset,
                    WHITE
                );

                arduboy.drawLine(
                    centerX + TILE_QUARTER,
                    centerY + offset,
                    centerX + TILE_HALF,
                    centerY + offset,
                    WHITE
                );

                break;
        }
    }
}
