#pragma once

#include <stdint.h>

#include "Types.h"

class Monster {
    public:
        void spawn(int8_t x, int8_t y, uint8_t hp);

        bool isAlive() const;

        Position getPosition() const;
        uint8_t getHp() const;

        void takeDamage(uint8_t damage);

        void begin();
        void update();

    private:
        Position position { 0,0 };

        uint8_t hp = 0;
        bool alive = false;
};
