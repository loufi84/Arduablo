#include <Arduino.h>

#include "items/ItemGenerator.h"


Item ItemGenerator::generateWeapon(uint8_t depth) {
    Item item;


    // --------------------
    // Type d'arme
    // --------------------

    switch (random(0, 3)) {
        case 0:
            item.type = ItemType::DAGGER;
            break;

        case 1:
            item.type = ItemType::SWORD;
            break;

        default:
            item.type = ItemType::AXE;
            break;
    }


    // --------------------
    // Rareté
    // --------------------

    item.rarity = generateRarity();


    // --------------------
    // Affixes
    // --------------------

    switch (item.rarity) {

        case ItemRarity::COMMON:
            // Aucun affixe
            break;


        case ItemRarity::MAGIC:
            // Un préfixe OU un suffixe
            if (random(0, 2) == 0) {
                item.prefix = generatePrefix();
            }
            else {
                item.suffix = generateSuffix();
            }
            break;


        case ItemRarity::RARE:
            // Préfixe + suffixe
            item.prefix = generatePrefix();
            item.suffix = generateSuffix();
            break;
    }


    // --------------------
    // Dégâts
    // --------------------

    item.damage =
        getBaseDamage(item.type, depth)
        + getPrefixDamage(item.prefix)
        + getSuffixDamage(item.suffix);


    return item;
}


ItemRarity ItemGenerator::generateRarity() {
    const uint8_t roll =
        static_cast<uint8_t>(random(0, 100));


    if (roll < 70) {
        return ItemRarity::COMMON;
    }

    if (roll < 95) {
        return ItemRarity::MAGIC;
    }

    return ItemRarity::RARE;
}


ItemPrefix ItemGenerator::generatePrefix() {
    switch (random(0, 3)) {

        case 0:
            return ItemPrefix::SHARP;

        case 1:
            return ItemPrefix::BRUTAL;

        default:
            return ItemPrefix::SAVAGE;
    }
}


ItemSuffix ItemGenerator::generateSuffix() {
    switch (random(0, 3)) {

        case 0:
            return ItemSuffix::OF_MIGHT;

        case 1:
            return ItemSuffix::OF_POWER;

        default:
            return ItemSuffix::OF_DOOM;
    }
}


uint8_t ItemGenerator::getBaseDamage(
    ItemType type,
    uint8_t depth
) {
    const uint8_t depthBonus =
        depth / 4;


    switch (type) {

        case ItemType::DAGGER:
            return 1 + depthBonus;

        case ItemType::SWORD:
            return 2 + depthBonus;

        case ItemType::AXE:
            return 3 + depthBonus;

        default:
            return 0;
    }
}


uint8_t ItemGenerator::getPrefixDamage(
    ItemPrefix prefix
) {
    switch (prefix) {

        case ItemPrefix::SHARP:
            return 1;

        case ItemPrefix::BRUTAL:
            return 2;

        case ItemPrefix::SAVAGE:
            return 3;

        default:
            return 0;
    }
}


uint8_t ItemGenerator::getSuffixDamage(
    ItemSuffix suffix
) {
    switch (suffix) {

        case ItemSuffix::OF_MIGHT:
            return 1;

        case ItemSuffix::OF_POWER:
            return 2;

        case ItemSuffix::OF_DOOM:
            return 3;

        default:
            return 0;
    }
}
