#include <Arduino.h>
#include <Arduboy2.h>

#include "Config.h"
#include "Game.h"

#include "world/DungeonGenerator.h"
#include "world/Tile.h"

#include "systems/AI.h"

#include "items/ItemGenerator.h"


extern Arduboy2 arduboy;


namespace {

bool isBossType(
    MonsterType type
) {
    return
        type == MonsterType::BONE_WARDEN
        ||
        type == MonsterType::ABYSS_LORD;
}


bool hasLivingBoss(
    const Monster* monsters
) {
    for (
        uint8_t i = 0;
        i < MAX_MONSTERS;
        ++i
    ) {
        if (
            monsters[i].isAlive()
            &&
            isBossType(
                monsters[i].getType()
            )
        ) {
            return true;
        }
    }

    return false;
}

}


void Game::begin() {

    state = GameState::TITLE;

    depth = 1;

    inventorySelection = 0;
}


void Game::update() {

    switch (state) {

        case GameState::TITLE: {

            if (arduboy.justPressed(A_BUTTON)) {
                startNewGame();
            }

            break;
        }


        case GameState::PLAYING: {

            const int8_t killedMonster =
                player.update(
                    dungeon,
                    monsters,
                    MAX_MONSTERS
                );


            if (killedMonster >= 0) {

                Monster& monster =
                    monsters[killedMonster];


                player.addXp(
                    monster.getXpReward()
                );


                tryDropLoot(
                    monster.getPosition()
                );
            }


            if (arduboy.justPressed(B_BUTTON)) {

                const uint8_t before =
                    player.getInventoryCount();


                tryPickupItem();


                const uint8_t after =
                    player.getInventoryCount();


                if (before == after) {

                    inventorySelection = 0;

                    state =
                        GameState::INVENTORY;

                    break;
                }
            }


            const Position playerPosition =
                player.getPosition();


            if (
                dungeon.getTile(
                    playerPosition.x,
                    playerPosition.y
                )
                ==
                Tile::STAIRS_DOWN
            ) {

                if (!hasLivingBoss(monsters)) {

                    if (
                        depth
                        ==
                        FINAL_BOSS_DEPTH
                    ) {
                        state =
                            GameState::VICTORY;

                        break;
                    }


                    ++depth;

                    generateLevel();

                    break;
                }
            }


            for (
                uint8_t i = 0;
                i < MAX_MONSTERS;
                ++i
            ) {
                monsters[i].tickAnimation();
            }


            if (arduboy.everyXFrames(10)) {

                AI::updateMonsters(
                    dungeon,
                    player,
                    monsters,
                    MAX_MONSTERS
                );
            }


            camera.follow(
                player.getPosition()
            );


            if (!player.isAlive()) {

                state =
                    GameState::GAME_OVER;
            }


            break;
        }


        case GameState::INVENTORY: {

            updateInventory();

            break;
        }


        case GameState::TOWN: {

            break;
        }


        case GameState::GAME_OVER: {

            if (arduboy.justPressed(A_BUTTON)) {

                startNewGame();
            }

            break;
        }


        case GameState::VICTORY: {

            if (arduboy.justPressed(A_BUTTON)) {

                state =
                    GameState::TITLE;
            }

            break;
        }
    }
}


void Game::render() {

    arduboy.clear();


    switch (state) {

        case GameState::TITLE: {

            arduboy.setCursor(
                37,
                20
            );


            arduboy.print(
                F("ARDUABLO")
            );


            arduboy.setCursor(
                35,
                38
            );


            arduboy.print(
                F("[A] START")
            );


            break;
        }


        case GameState::PLAYING: {

            renderer.drawDungeon(
                dungeon,
                camera
            );


            renderer.drawGroundItem(
                groundItems,
                MAX_ITEMS,
                camera
            );


            renderer.drawMonsters(
                monsters,
                MAX_MONSTERS,
                camera
            );


            renderer.drawPlayer(
                player,
                camera
            );


            renderer.drawPlayerAttack(
                player,
                camera
            );


            renderer.drawMonsterAttack(
                monsters,
                MAX_MONSTERS,
                camera
            );


            hud.draw(
                player
            );


            break;
        }


        case GameState::INVENTORY: {

            hud.drawInventory(
                player,
                inventorySelection
            );


            break;
        }


        case GameState::TOWN: {

            break;
        }


        case GameState::GAME_OVER: {

            arduboy.setCursor(
                38,
                22
            );


            arduboy.print(
                F("YOU DIED")
            );


            arduboy.setCursor(
                23,
                38
            );


            arduboy.print(
                F("[A] TRY AGAIN")
            );


            break;
        }


        case GameState::VICTORY: {

            arduboy.setCursor(
                40,
                20
            );


            arduboy.print(
                F("VICTORY")
            );


            arduboy.setCursor(
                29,
                38
            );


            arduboy.print(
                F("[A] TITLE")
            );


            break;
        }
    }
}


void Game::startNewGame() {

    depth = 1;

    inventorySelection = 0;


    player.begin();


    generateLevel();


    state =
        GameState::PLAYING;
}


void Game::generateLevel() {

    DungeonGenerator::generate(
        dungeon
    );


    player.setPosition(
        dungeon.getStartPosition()
    );


    for (
        uint8_t i = 0;
        i < MAX_MONSTERS;
        ++i
    ) {
        monsters[i] =
            Monster();
    }


    for (
        uint8_t i = 0;
        i < MAX_ITEMS;
        ++i
    ) {
        groundItems[i] =
            GroundItem {};
    }


    const Position playerPosition =
        player.getPosition();


    if (
        depth == MID_BOSS_DEPTH
        ||
        depth == FINAL_BOSS_DEPTH
    ) {

        const MonsterType bossType =
            depth == MID_BOSS_DEPTH
                ?
                MonsterType::BONE_WARDEN
                :
                MonsterType::ABYSS_LORD;


        const Position exitPosition =
            dungeon.getExitPosition();


        const int8_t offsets[8][2] = {
            {-1,  0},
            { 1,  0},
            { 0, -1},
            { 0,  1},
            {-1, -1},
            { 1, -1},
            {-1,  1},
            { 1,  1}
        };


        bool bossSpawned = false;


        for (
            uint8_t i = 0;
            i < 8;
            ++i
        ) {

            const int16_t x =
                static_cast<int16_t>(
                    exitPosition.x
                )
                +
                offsets[i][0];


            const int16_t y =
                static_cast<int16_t>(
                    exitPosition.y
                )
                +
                offsets[i][1];


            if (
                x < 0
                ||
                y < 0
                ||
                x >= MAP_WIDTH
                ||
                y >= MAP_HEIGHT
            ) {
                continue;
            }


            if (
                dungeon.getTile(
                    static_cast<uint8_t>(x),
                    static_cast<uint8_t>(y)
                )
                !=
                Tile::FLOOR
            ) {
                continue;
            }


            monsters[0].spawn(
                static_cast<uint8_t>(x),
                static_cast<uint8_t>(y),
                bossType,
                depth
            );


            bossSpawned = true;

            break;
        }


        if (!bossSpawned) {

            uint8_t attempts = 0;


            while (attempts < 100) {

                ++attempts;


                Position position;


                if (
                    !DungeonGenerator::findRandomFloor(
                        dungeon,
                        position
                    )
                ) {
                    continue;
                }


                if (
                    abs(
                        position.x
                        -
                        playerPosition.x
                    )
                    <= 2
                    &&
                    abs(
                        position.y
                        -
                        playerPosition.y
                    )
                    <= 2
                ) {
                    continue;
                }


                monsters[0].spawn(
                    position.x,
                    position.y,
                    bossType,
                    depth
                );


                bossSpawned = true;

                break;
            }
        }


        camera.follow(
            player.getPosition()
        );


        return;
    }


    constexpr uint8_t MONSTERS_PER_LEVEL =
        3;


    uint8_t spawned = 0;

    uint8_t attempts = 0;


    while (
        spawned < MONSTERS_PER_LEVEL
        &&
        attempts < 100
    ) {

        ++attempts;


        Position position;


        if (
            !DungeonGenerator::findRandomFloor(
                dungeon,
                position
            )
        ) {
            continue;
        }


        if (
            abs(
                position.x
                -
                playerPosition.x
            )
            <= 2
            &&
            abs(
                position.y
                -
                playerPosition.y
            )
            <= 2
        ) {
            continue;
        }


        bool occupied = false;


        for (
            uint8_t i = 0;
            i < spawned;
            ++i
        ) {

            if (!monsters[i].isAlive()) {
                continue;
            }


            const Position monsterPosition =
                monsters[i].getPosition();


            if (
                monsterPosition.x
                    ==
                position.x
                &&
                monsterPosition.y
                    ==
                position.y
            ) {

                occupied = true;

                break;
            }
        }


        if (occupied) {
            continue;
        }


        monsters[spawned].spawn(
            position.x,
            position.y,
            rollMonsterType(),
            depth
        );


        ++spawned;
    }


    camera.follow(
        player.getPosition()
    );
}


MonsterType Game::rollMonsterType() const {

    const uint8_t roll =
        static_cast<uint8_t>(
            random(0, 100)
        );


    if (depth < 3) {

        return
            MonsterType::ZOMBIE;
    }


    if (depth < 5) {

        if (roll < 70) {

            return
                MonsterType::ZOMBIE;
        }


        return
            MonsterType::SKELETON;
    }


    if (roll < 50) {

        return
            MonsterType::ZOMBIE;
    }


    if (roll < 80) {

        return
            MonsterType::SKELETON;
    }


    return
        MonsterType::BRUTE;
}


void Game::tryDropLoot(
    Position position
) {

    if (random(0, 100) >= 50) {
        return;
    }


    for (
        uint8_t i = 0;
        i < MAX_ITEMS;
        ++i
    ) {

        if (groundItems[i].active) {
            continue;
        }


        groundItems[i].item =
            ItemGenerator::generateWeapon(
                depth
            );


        groundItems[i].position =
            position;


        groundItems[i].active =
            true;


        return;
    }
}


void Game::tryPickupItem() {

    const Position playerPosition =
        player.getPosition();


    for (
        uint8_t i = 0;
        i < MAX_ITEMS;
        ++i
    ) {

        if (!groundItems[i].active) {
            continue;
        }


        if (
            groundItems[i].position.x
                !=
            playerPosition.x
            ||
            groundItems[i].position.y
                !=
            playerPosition.y
        ) {
            continue;
        }


        if (
            player.addItem(
                groundItems[i].item
            )
        ) {

            groundItems[i].active =
                false;
        }


        return;
    }
}


void Game::updateInventory() {

    const uint8_t count =
        player.getInventoryCount();


    if (arduboy.justPressed(B_BUTTON)) {

        state =
            GameState::PLAYING;


        return;
    }


    if (count == 0) {

        inventorySelection = 0;


        return;
    }


    if (arduboy.justPressed(UP_BUTTON)) {

        if (inventorySelection == 0) {

            inventorySelection =
                count - 1;
        }

        else {

            --inventorySelection;
        }
    }


    if (arduboy.justPressed(DOWN_BUTTON)) {

        ++inventorySelection;


        if (
            inventorySelection
            >=
            count
        ) {

            inventorySelection = 0;
        }
    }


    if (arduboy.justPressed(A_BUTTON)) {

        player.equipInventoryItem(
            inventorySelection
        );


        const uint8_t newCount =
            player.getInventoryCount();


        if (newCount == 0) {

            inventorySelection = 0;
        }

        else if (
            inventorySelection
            >=
            newCount
        ) {

            inventorySelection =
                newCount - 1;
        }
    }
}
