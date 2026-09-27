#pragma once

#include <stdint.h>

#include "items/Item.h"

class ItemGenerator {
public:
    static Item generateWeapon(uint8_t depth);
};
