#pragma once

#include <stdint.h>
#include "Types.h"

class Dungeon;
class Monster;

class Player {
public:
    void begin();

    void update(
        const Dungeon& dungeon,
        Monster* monsters,
        uint8_t monsterCount
    );

    Position getPosition() const;
    Direction getDirection() const;

private:
    Position position { 2, 2 };
    Direction direction = Direction::DOWN;

    void tryMove(
        int8_t dx,
        int8_t dy,
        const Dungeon& dungeon,
        const Monster* monsters,
        uint8_t monsterCount
    );

    void attack(
        Monster* monsters,
        uint8_t monsterCount
    );
};
