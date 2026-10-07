#pragma once

#include <stdint.h>

#include "Types.h"
#include "data/MonsterData.h"


class Monster {
public:

    void spawn(
        int8_t x,
        int8_t y,
        MonsterType type,
        uint8_t depth
    );


    bool isAlive() const;
    bool isBoss() const;

    Position getPosition() const;

    void setPosition(
        int8_t x,
        int8_t y
    );


    MonsterType getType() const;


    uint8_t getHp() const;

    uint8_t getDamage() const;

    uint8_t getXpReward() const;

    uint8_t getActionDelay() const;


    bool takeDamage(
        uint8_t damage
    );


    void tickAnimation();

    void startAttack(
        Direction direction
    );

    bool isAttacking() const;

    Direction getAttackDirection() const;


private:

    Position position { 0, 0 };


    MonsterType type =
        MonsterType::ZOMBIE;


    uint8_t depth = 1;

    uint8_t hp = 0;


    bool alive = false;


    uint8_t attackTimer = 0;

    Direction attackDirection =
        Direction::DOWN;
};
