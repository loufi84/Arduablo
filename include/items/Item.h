#pragma once

#include <stdint.h>

enum class ItemType : uint8_t {
    NONE = 0,
    SWORD,
    AXE
};

struct Item {
    ItemType type = ItemType::NONE;

    uint8_t damage = 0;
    uint8_t prefix = 0;
    uint8_t suffix = 0;

    bool isValid() const {
        return type != ItemType::NONE;
    }
};
