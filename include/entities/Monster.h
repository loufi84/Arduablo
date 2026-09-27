#pragma once

#include <stdint.h>

#include "Types.h"

class Monster {
    public:
        void spawn(int8_t x, int8_t y, uint8_t hp);

        bool isAlive() const;

        Position getPosition() const;
        uint8_t getHp() const;

        bool takeDamage(uint8_t damage);

        void begin();
        void update();

        void setPosition(int8_t x, int8_t y);

        void tickAnimation();

        void startAttack(Direction direction);

        bool isAttacking() const;
        Direction getAttackDirection() const;

    private:
        Position position { 0,0 };

        uint8_t hp = 0;
        bool alive = false;

        uint8_t attackTimer = 0;
        Direction attackDirection = Direction::DOWN;
};
