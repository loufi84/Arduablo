#include <stdint.h>

#include "Game.h"

void Game::begin() {
    dungeon.begin();
    player.begin();

    for (uint8_t i = 0; i < MAX_MONSTERS; i++) {
        monsters[i].begin();
    }
}

void Game::update() {
    switch(state) {
        case GameState::TITLE:
            break;

        case GameState::PLAYING:
            dungeon.update();
            player.update();

            for (uint8_t i = 0; i < MAX_MONSTERS; i++) {
                monsters[i].update();
            }
            break;

        case GameState::INVENTORY:
            break;
        
        case GameState::TOWN:
            break;

        case GameState::GAME_OVER:
            break;
    }
}

void Game::render() {

}
