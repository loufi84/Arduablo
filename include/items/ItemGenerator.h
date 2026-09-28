#pragma once

#include <stdint.h>

#include "items/Item.h"


class ItemGenerator {
public:
    static Item generateWeapon(uint8_t depth);

private:
    static ItemRarity generateRarity();

    static ItemPrefix generatePrefix();
    static ItemSuffix generateSuffix();

    static uint8_t getBaseDamage(
        ItemType type,
        uint8_t depth
    );

    static uint8_t getPrefixDamage(
        ItemPrefix prefix
    );

    static uint8_t getSuffixDamage(
        ItemSuffix suffix
    );
};
