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
            uint8_t monsterCounr
        );

    private:
        static void updateZombie(
            const Dungeon& dungeon,
            Player& player,
            Monster& monster,
            const Monster* monsters,
            uint8_t monsterCount
        );

        static bool isOccupied(
            int8_t x,
            int8_t y,
            const Monster* monsters,
            uint8_t monsterCount,
            const Monster* ignoredMonster
        );
};
