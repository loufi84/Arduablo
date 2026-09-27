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

    uint8_t getHp() const;
    uint8_t getMaxHp() const;

    bool isAlive() const;
    void takeDamage(uint8_t damage);

private:
    Position position { 2, 2 };
    Direction direction = Direction::DOWN;

    uint8_t hp = 5;
    uint8_t maxHp = 5;

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
