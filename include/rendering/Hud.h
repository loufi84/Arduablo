#pragma once

#include <stdint.h>

#include "items/Item.h"

class Player;

class Hud {
    public:
        void draw(const Player& player);

        void drawInventory(const Player& player, uint8_t selectedIndex);

    private:
        void drawItemName(const Item& item);
};
