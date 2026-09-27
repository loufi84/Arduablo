#include <Arduino.h>

#include "items/ItemGenerator.h"

Item ItemGenerator::generateWeapon(uint8_t depth) {
    Item item;

    if (random(0, 2) == 0) {
        item.type = ItemType::SWORD;

        item.damage =
            1 + random(0, 2) + depth / 3;
    }
    else {
        item.type = ItemType::AXE;

        item.damage =
            2 + random(0, 2) + depth / 3;
    }

    return item;
}
