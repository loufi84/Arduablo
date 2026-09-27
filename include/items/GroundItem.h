#pragma once

#include "Types.h"
#include "items/Item.h"

struct GroundItem {
    Item item;
    Position position { 0, 0 };

    bool active = false;
};
