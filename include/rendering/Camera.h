#pragma once

#include <stdint.h>

#include "Types.h"

class Camera {
    public:
        void follow(Position target);

        uint8_t getX() const;
        uint8_t getY() const;

    private:
        uint8_t x = 0;
        uint8_t y = 0;
};
