#include <Arduboy2.h>

#include "Game.h"
#include "systems/AI.h"

extern Arduboy2 arduboy;

void Game::begin() {
    state = GameState::TITLE;
}

void Game::update() {
    switch (state) {
        case GameState::TITLE:
            if (arduboy.justPressed(A_BUTTON)) {
                dungeon.begin();
                player.begin();
                monsters[0].spawn(8, 3, 3);
                state = GameState::PLAYING;
            }
            break;

        case GameState::PLAYING:
            player.update(dungeon, monsters, MAX_MONSTERS);

            if (arduboy.everyXFrames(10)) {
                AI::updateMonsters(dungeon, player, monsters, MAX_MONSTERS);
            }

            if (!player.isAlive()) {
                state = GameState::GAME_OVER;
            }

            break;

        case GameState::GAME_OVER:
        if (arduboy.justPressed(A_BUTTON)) {
            dungeon.begin();
            player.begin();

            for (uint8_t i = 0; i < MAX_MONSTERS; ++i) {
                monsters[i] = Monster();
            }

            monsters[0].spawn(8, 3, 3);

            state = GameState::PLAYING;
        }

        break;

        default:
            break;
    }
}

void Game::render() {
    arduboy.clear();

    switch (state) {
        case GameState::TITLE:
            arduboy.setCursor(38, 22);
            arduboy.print(F("ARDUABLO"));

            arduboy.setCursor(32, 40);
            arduboy.print(F("[A] START"));
            break;

        case GameState::PLAYING:
            renderer.drawDungeon(dungeon);
            renderer.drawPlayer(player);
            renderer.drawMonsters(monsters, MAX_MONSTERS);
            hud.draw(player);
            break;

        case GameState::GAME_OVER:
            arduboy.setCursor(34, 20);
            arduboy.print(F("YOU DIED"));

            arduboy.setCursor(20, 38);
            arduboy.print(F("[A] TRY AGAIN"));
            break;

        default:
            break;
    }
}
