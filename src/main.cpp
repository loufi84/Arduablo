#include <Arduboy2.h>
#include "Game.h"

Arduboy2 arduboy;
Game game;

void setup() {
    arduboy.begin();
    arduboy.setFrameRate(30);

    game.begin();
}

void loop() {
    if (!arduboy.nextFrame()) {
        return;
    }

    arduboy.pollButtons();

    game.update();
    game.render();

    arduboy.display();
}
