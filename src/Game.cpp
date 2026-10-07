#include <Arduino.h>
#include <Arduboy2.h>

#include "Config.h"
#include "Game.h"

#include "world/DungeonGenerator.h"
#include "world/Tile.h"

#include "systems/AI.h"

#include "items/ItemGenerator.h"


extern Arduboy2 arduboy;


// Initialisation

void Game::begin() {
    state = GameState::TITLE;

    depth = 1;

    inventorySelection = 0;
}


// Update

void Game::update() {

    switch (state) {

        // TITLE

        case GameState::TITLE: {

            if (arduboy.justPressed(A_BUTTON)) {
                startNewGame();
            }

            break;
        }


        // PLAYING

        case GameState::PLAYING: {

            // Joueur

            const int8_t killedMonster =
                player.update(
                    dungeon,
                    monsters,
                    MAX_MONSTERS
                );


            // Monstre tué

            if (killedMonster >= 0) {

                Monster& monster =
                    monsters[killedMonster];


                // XP spécifique au type
                player.addXp(
                    monster.getXpReward()
                );


                // Loot sur la position
                // du monstre mort
                tryDropLoot(
                    monster.getPosition()
                );
            }


            // Ramassage / inventaire

            if (arduboy.justPressed(B_BUTTON)) {

                const uint8_t before =
                    player.getInventoryCount();


                tryPickupItem();


                const uint8_t after =
                    player.getInventoryCount();


                // Rien ramassé :
                // ouverture de l'inventaire
                if (before == after) {

                    inventorySelection = 0;

                    state =
                        GameState::INVENTORY;

                    break;
                }
            }


            // Escalier

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
                ++depth;

                generateLevel();

                break;
            }


            // Animations monstres

            for (
                uint8_t i = 0;
                i < MAX_MONSTERS;
                ++i
            ) {
                monsters[i].tickAnimation();
            }


            // IA

            // Pour l'instant on garde
            // encore l'ancien rythme global.
            //
            // getActionDelay() sera utilisé
            // dans la prochaine étape.

            if (arduboy.everyXFrames(10)) {

                AI::updateMonsters(
                    dungeon,
                    player,
                    monsters,
                    MAX_MONSTERS
                );
            }


            // Caméra

            camera.follow(
                player.getPosition()
            );


            // Mort

            if (!player.isAlive()) {

                state =
                    GameState::GAME_OVER;
            }


            break;
        }


        // INVENTORY

        case GameState::INVENTORY: {

            updateInventory();

            break;
        }


        // GAME OVER

        case GameState::GAME_OVER: {

            if (arduboy.justPressed(A_BUTTON)) {

                startNewGame();
            }

            break;
        }
    }
}


// Render

void Game::render() {

    arduboy.clear();


    switch (state) {

        // TITLE

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


        // PLAYING

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


        // INVENTORY

        case GameState::INVENTORY: {

            hud.drawInventory(
                player,
                inventorySelection
            );


            break;
        }


        // GAME OVER

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
    }
}


// Nouvelle partie

void Game::startNewGame() {

    depth = 1;

    inventorySelection = 0;


    player.begin();


    generateLevel();


    state =
        GameState::PLAYING;
}


// Génération d'un étage

void Game::generateLevel() {

    // Donjon
    DungeonGenerator::generate(
        dungeon
    );


    // Position joueur

    player.setPosition(
        dungeon.getStartPosition()
    );


    // Reset monstres

    for (
        uint8_t i = 0;
        i < MAX_MONSTERS;
        ++i
    ) {
        monsters[i] =
            Monster();
    }


    // Reset objets au sol

    for (
        uint8_t i = 0;
        i < MAX_ITEMS;
        ++i
    ) {
        groundItems[i] =
            GroundItem {};
    }


    // Spawn monstres

    constexpr uint8_t MONSTERS_PER_LEVEL =
        3;


    uint8_t spawned = 0;

    uint8_t attempts = 0;


    const Position playerPosition =
        player.getPosition();


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


        // Pas trop près du joueur

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


        // Pas deux monstres
        // sur la même case

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


        // Création du monstre

        monsters[spawned].spawn(
            position.x,
            position.y,
            rollMonsterType(),
            depth
        );


        ++spawned;
    }


    // Caméra

    camera.follow(
        player.getPosition()
    );
}


// Choix du type de monstre

MonsterType Game::rollMonsterType() const {

    const uint8_t roll =
        static_cast<uint8_t>(
            random(0, 100)
        );


    // Depth 1 - 2
    //
    // 100 % Zombie

    if (depth < 3) {

        return
            MonsterType::ZOMBIE;
    }


    // Depth 3 - 4
    //
    // 70 % Zombie
    // 30 % Skeleton

    if (depth < 5) {

        if (roll < 70) {

            return
                MonsterType::ZOMBIE;
        }


        return
            MonsterType::SKELETON;
    }


    // Depth 5+
    //
    // 50 % Zombie
    // 30 % Skeleton
    // 20 % Brute

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


// Drop de loot

void Game::tryDropLoot(
    Position position
) {

    // 50 % de chance de drop
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


// Ramassage

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


        // On ne fait disparaître l'objet
        // que si le sac a effectivement
        // réussi à le prendre.
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


// Inventaire

void Game::updateInventory() {

    const uint8_t count =
        player.getInventoryCount();


    // Retour au jeu

    if (arduboy.justPressed(B_BUTTON)) {

        state =
            GameState::PLAYING;

        return;
    }


    // Inventaire vide

    if (count == 0) {

        inventorySelection = 0;

        return;
    }


    // Sélection précédente

    if (arduboy.justPressed(UP_BUTTON)) {

        if (inventorySelection == 0) {

            inventorySelection =
                count - 1;
        }

        else {

            --inventorySelection;
        }
    }


    // Sélection suivante

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


    // Equiper

    if (arduboy.justPressed(A_BUTTON)) {

        player.equipInventoryItem(
            inventorySelection
        );


        // Le nombre d'objets peut changer
        // si aucune arme n'était équipée.
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
