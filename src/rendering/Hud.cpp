#include <Arduboy2.h>

#include "rendering/Hud.h"
#include "entities/Player.h"

extern Arduboy2 arduboy;

void Hud::draw(const Player& player) {
    arduboy.setCursor(1, 56);

    arduboy.print(F("HP "));
    arduboy.print(player.getHp());
    arduboy.print("/");
    arduboy.print(player.getMaxHp());
}
