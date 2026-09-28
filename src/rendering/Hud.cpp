#include <Arduboy2.h>

#include "rendering/Hud.h"
#include "entities/Player.h"

extern Arduboy2 arduboy;

void Hud::draw(const Player& player) {
    arduboy.setCursor(1, 56);

    arduboy.print(F("HP "));
    arduboy.print(player.getHp());
    arduboy.print("/");
    arduboy.print(player.getMaxHp());

    arduboy.setCursor(55, 56);

    arduboy.print(F("DMG "));
    arduboy.print(
        player.getAttackDamage()
    );
}

void Hud::drawItemName(const Item& item) {

    // Préfixe
    switch (item.prefix) {

        case ItemPrefix::SHARP:
            arduboy.print(F("Sharp "));
            break;

        case ItemPrefix::BRUTAL:
            arduboy.print(F("Brutal "));
            break;

        case ItemPrefix::SAVAGE:
            arduboy.print(F("Savage "));
            break;

        default:
            break;
    }


    // Base
    switch (item.type) {

        case ItemType::SWORD:
            arduboy.print(F("Sword"));
            break;

        case ItemType::AXE:
            arduboy.print(F("Axe"));
            break;

        default:
            arduboy.print(F("???"));
            break;
    }


    // Suffixe
    switch (item.suffix) {

        case ItemSuffix::OF_MIGHT:
            arduboy.print(F(" of Might"));
            break;

        case ItemSuffix::OF_POWER:
            arduboy.print(F(" of Power"));
            break;

        case ItemSuffix::OF_DOOM:
            arduboy.print(F(" of Doom"));
            break;

        default:
            break;
    }

    switch (item.rarity) {

        case ItemRarity::COMMON:
            arduboy.print(' ');
            break;

        case ItemRarity::MAGIC:
            arduboy.print('*');
            break;

        case ItemRarity::RARE:
            arduboy.print('!');
            break;
    }
}

void Hud::drawInventory(
    const Player& player,
    uint8_t selectedIndex
) {
    const uint8_t count =
        player.getInventoryCount();

    arduboy.setCursor(0, 0);
    arduboy.print(F("INVENTORY "));
    arduboy.print(count);
    arduboy.print(F("/6"));

    if (count == 0) {
        arduboy.setCursor(28, 24);
        arduboy.print(F("EMPTY"));

        arduboy.setCursor(0, 56);
        arduboy.print(F("[B] Back"));

        return;
    }

    // On affiche au maximum 4 objets à la fois.
    uint8_t firstVisible = 0;

    if (selectedIndex >= 4) {
        firstVisible = selectedIndex - 3;
    }

    for (
        uint8_t row = 0;
        row < 4;
        ++row
    ) {
        const uint8_t itemIndex =
            firstVisible + row;

        if (itemIndex >= count) {
            break;
        }

        const Item& item =
            player.getInventoryItem(itemIndex);

        const uint8_t y =
            10 + row * 9;

        arduboy.setCursor(0, y);

        if (itemIndex == selectedIndex) {
            arduboy.print('>');
        }
        else {
            arduboy.print(' ');
        }

        drawItemName(item);

        arduboy.print(F(" +"));
        arduboy.print(item.damage);
    }

    // Arme équipée
    arduboy.setCursor(0, 47);
    arduboy.print(F("EQ: "));

    const Item& equipped =
        player.getEquipment().getWeapon();

    if (equipped.isValid()) {
        drawItemName(equipped);

        arduboy.print(F(" +"));
        arduboy.print(equipped.damage);
    }
    else {
        arduboy.print(F("None"));
    }

    arduboy.setCursor(0, 56);
    arduboy.print(F("A Equip  B Back"));
}
