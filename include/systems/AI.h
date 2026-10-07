#pragma once

#include <stdint.h>

class Dungeon;
class Player;
class Monster;

class AI {
public:
    static void updateMonsters(
        const Dungeon& dungeon,
        Player& player,
        Monster* monsters,
        uint8_t monsterCount
    );
};