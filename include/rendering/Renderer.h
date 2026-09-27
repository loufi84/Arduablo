#pragma once

#include <stdint.h>

class Dungeon;
class Player;
class Monster;

class Renderer {
    public:
        void drawDungeon(const Dungeon& dungeon);
        void drawPlayer(const Player& player);
        void drawMonsters(const Monster* monsters, uint8_t monsterCount);
};
