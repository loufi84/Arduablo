#include <Arduboy2.h>
#include "Game.h"

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
            break;

        default:
            break;
    }
}
