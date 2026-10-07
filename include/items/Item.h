#pragma once

#include <stdint.h>


enum class ItemType : uint8_t {
    NONE = 0,
    DAGGER,
    SWORD,
    AXE
};


enum class ItemRarity : uint8_t {
    COMMON = 0,
    MAGIC,
    RARE
};


enum class ItemPrefix : uint8_t {
    NONE = 0,

    SHARP,      // +1 DMG
    BRUTAL,     // +2 DMG
    SAVAGE      // +3 DMG
};


enum class ItemSuffix : uint8_t {
    NONE = 0,

    OF_MIGHT,   // +1 DMG
    OF_POWER,   // +2 DMG
    OF_DOOM     // +3 DMG
};


struct Item {
    ItemType type = ItemType::NONE;

    ItemRarity rarity = ItemRarity::COMMON;

    ItemPrefix prefix = ItemPrefix::NONE;
    ItemSuffix suffix = ItemSuffix::NONE;

    uint8_t damage = 0;


    bool isValid() const {
        return type != ItemType::NONE;
    }
};
