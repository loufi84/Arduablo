#include <Arduboy2.h>
#include <Arduino.h>

#include "Game.h"
#include "systems/AI.h"
#include "world/DungeonGenerator.h"
#include "items/ItemGenerator.h"

extern Arduboy2 arduboy;


void Game::begin() {
    state = GameState::TITLE;
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
                tryDropLoot(
                    monsters[killedMonster].getPosition()
                );
            }

            // Vérifie si le joueur vient de marcher
            // sur l'escalier vers l'étage suivant
            const Position playerPos =
                player.getPosition();

            if (
                dungeon.getTile(
                    playerPos.x,
                    playerPos.y
                ) == Tile::STAIRS_DOWN
            ) {
                ++depth;

                generateLevel();

                break;
            }


            // Animations des monstres
            for (
                uint8_t i = 0;
                i < MAX_MONSTERS;
                ++i
            ) {
                monsters[i].tickAnimation();
            }


            // IA volontairement plus lente que le framerate
            if (arduboy.everyXFrames(10)) {
                AI::updateMonsters(
                    dungeon,
                    player,
                    monsters,
                    MAX_MONSTERS
                );
            }


            // La caméra suit le joueur
            camera.follow(
                player.getPosition()
            );

            // Récupération du loot
            if (arduboy.justPressed(B_BUTTON)) {
                tryPickupItem();
            }


            // Mort du joueur
            if (!player.isAlive()) {
                state = GameState::GAME_OVER;
            }

            break;
        }


        case GameState::GAME_OVER: {
            if (arduboy.justPressed(A_BUTTON)) {
                startNewGame();
            }

            break;
        }


        default:
            break;
    }
}


void Game::render() {
    arduboy.clear();

    switch (state) {

        case GameState::TITLE: {
            arduboy.setCursor(38, 22);
            arduboy.print(F("ARDUABLO"));

            arduboy.setCursor(32, 40);
            arduboy.print(F("[A] START"));

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

            renderer.drawPlayer(
                player,
                camera
            );

            renderer.drawMonsters(
                monsters,
                MAX_MONSTERS,
                camera
            );


            // Overlays d'attaque dessinés après
            // les personnages pour apparaître au-dessus

            renderer.drawPlayerAttack(
                player,
                camera
            );

            renderer.drawMonsterAttack(
                monsters,
                MAX_MONSTERS,
                camera
            );


            hud.draw(player);

            break;
        }


        case GameState::GAME_OVER: {
            arduboy.setCursor(34, 20);
            arduboy.print(F("YOU DIED"));

            arduboy.setCursor(20, 38);
            arduboy.print(F("[A] TRY AGAIN"));

            break;
        }


        default:
            break;
    }
}


void Game::startNewGame() {
    depth = 1;

    // Réinitialise le personnage :
    // HP, position par défaut, direction, etc.
    player.begin();

    generateLevel();

    state = GameState::PLAYING;
}


void Game::generateLevel() {
    // Génère murs, salles, couloirs et escalier
    DungeonGenerator::generate(dungeon);

    // Génération du loot
    for (uint8_t i = 0; i < MAX_ITEMS; ++i) {
    groundItems[i] = GroundItem {};
    }


    // Place le joueur au centre de la première salle
    player.setPosition(
        dungeon.getStartPosition()
    );


    // Nettoyage des monstres de l'étage précédent
    for (
        uint8_t i = 0;
        i < MAX_MONSTERS;
        ++i
    ) {
        monsters[i] = Monster();
    }


    // Pour le moment : 3 zombies par niveau
    constexpr uint8_t SPAWN_COUNT = 3;

    uint8_t spawned = 0;
    uint8_t attempts = 0;

    // La limite d'essais évite une boucle infinie
    // si aucun emplacement convenable n'est trouvé.
    while (
        spawned < SPAWN_COUNT &&
        attempts < 100
    ) {
        ++attempts;

        Position spawn;

        if (!DungeonGenerator::findRandomFloor(
            dungeon,
            spawn
        )) {
            break;
        }


        const Position playerPos =
            player.getPosition();

        const int8_t dx =
            spawn.x - playerPos.x;

        const int8_t dy =
            spawn.y - playerPos.y;


        // Pas de zombie juste à côté du point de départ
        if (
            dx >= -2 &&
            dx <= 2 &&
            dy >= -2 &&
            dy <= 2
        ) {
            continue;
        }


        // Évite deux zombies sur la même case
        bool occupied = false;

        for (
            uint8_t i = 0;
            i < spawned;
            ++i
        ) {
            const Position monsterPos =
                monsters[i].getPosition();

            if (
                monsterPos.x == spawn.x &&
                monsterPos.y == spawn.y
            ) {
                occupied = true;
                break;
            }
        }

        if (occupied) {
            continue;
        }


        monsters[spawned].spawn(
            spawn.x,
            spawn.y,
            3
        );

        ++spawned;
    }


    // Centre immédiatement la caméra sur le nouveau spawn
    camera.follow(
        player.getPosition()
    );
}

void Game::tryDropLoot(Position position) {
    // Pour le prototype : 50 % de chance
    if (random(0, 100) >= 50) {
        return;
    }

    for (uint8_t i = 0; i < MAX_ITEMS; ++i) {
        if (groundItems[i].active) {
            continue;
        }

        groundItems[i].item =
            ItemGenerator::generateWeapon(depth);

        groundItems[i].position =
            position;

        groundItems[i].active = true;

        return;
    }
}

void Game::tryPickupItem() {
    const Position playerPos =
        player.getPosition();

    for (uint8_t i = 0; i < MAX_ITEMS; ++i) {
        if (!groundItems[i].active) {
            continue;
        }

        if (
            groundItems[i].position.x != playerPos.x ||
            groundItems[i].position.y != playerPos.y
        ) {
            continue;
        }

        player.equip(
            groundItems[i].item
        );

        groundItems[i].active = false;

        return;
    }
}
