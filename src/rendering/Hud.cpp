#include <Arduboy2.h>

#include "Config.h"

#include "rendering/Hud.h"

#include "entities/Player.h"
#include "items/Item.h"


extern Arduboy2 arduboy;


// ============================================================
// HUD en jeu
// ============================================================

void Hud::draw(
    const Player& player
) {
    const uint8_t hudY =
        VIEW_HEIGHT * TILE_SIZE;


    // --------------------
    // Ligne 1
    // --------------------

    arduboy.setCursor(
        0,
        hudY
    );

    arduboy.print(F("HP "));

    arduboy.print(
        player.getHp()
    );

    arduboy.print('/');

    arduboy.print(
        player.getMaxHp()
    );


    arduboy.print(F(" D"));

    arduboy.print(
        player.getAttackDamage()
    );


    arduboy.print(F(" L"));

    arduboy.print(
        player.getLevel()
    );


    // --------------------
    // Ligne 2
    // si on a encore la place
    // --------------------

    if (hudY + 8 < 64) {

        arduboy.setCursor(
            0,
            hudY + 8
        );

        arduboy.print(F("XP "));

        arduboy.print(
            player.getXp()
        );

        arduboy.print('/');

        arduboy.print(
            player.getXpToNextLevel()
        );
    }
}


// ============================================================
// Nom d'un objet
// ============================================================

void Hud::drawItemName(
    const Item& item
) {
    switch (item.type) {

        case ItemType::DAGGER:
            arduboy.print(F("Dagger"));
            break;

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
}


// ============================================================
// Inventaire
// ============================================================

void Hud::drawInventory(
    const Player& player,
    uint8_t selectedIndex
) {
    const uint8_t count =
        player.getInventoryCount();


    // --------------------
    // Titre
    // --------------------

    arduboy.setCursor(
        0,
        0
    );

    arduboy.print(F("INVENTORY "));

    arduboy.print(count);

    arduboy.print(F("/6"));


    // --------------------
    // Inventaire vide
    // --------------------

    if (count == 0) {

        arduboy.setCursor(
            46,
            24
        );

        arduboy.print(
            F("EMPTY")
        );


        arduboy.setCursor(
            0,
            56
        );

        arduboy.print(
            F("[B] Back")
        );

        return;
    }


    // ========================================================
    // Liste
    //
    // 4 objets visibles simultanément.
    // ========================================================

    uint8_t firstVisible = 0;


    if (selectedIndex >= 4) {

        firstVisible =
            selectedIndex - 3;
    }


    for (
        uint8_t row = 0;
        row < 4;
        ++row
    ) {
        const uint8_t index =
            firstVisible + row;


        if (index >= count) {
            break;
        }


        const Item& item =
            player.getInventoryItem(
                index
            );


        const uint8_t y =
            9 + row * 9;


        arduboy.setCursor(
            0,
            y
        );


        // --------------------
        // Curseur
        // --------------------

        if (index == selectedIndex) {

            arduboy.print('>');
        }
        else {

            arduboy.print(' ');
        }


        // --------------------
        // Rareté
        // --------------------

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


        // --------------------
        // Nom
        // --------------------

        drawItemName(
            item
        );


        // --------------------
        // Dégâts
        // --------------------

        arduboy.print(F(" +"));

        arduboy.print(
            item.damage
        );
    }


    // ========================================================
    // Arme équipée
    // ========================================================

    arduboy.setCursor(
        0,
        47
    );

    arduboy.print(F("EQ: "));


    const Item& equipped =
        player
            .getEquipment()
            .getWeapon();


    if (equipped.isValid()) {

        drawItemName(
            equipped
        );

        arduboy.print(F(" +"));

        arduboy.print(
            equipped.damage
        );
    }
    else {

        arduboy.print(
            F("None")
        );
    }


    // ========================================================
    // Commandes
    // ========================================================

    arduboy.setCursor(
        0,
        56
    );

    arduboy.print(
        F("A Equip  B Back")
    );
}
