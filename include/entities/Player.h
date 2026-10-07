#pragma once

#include <stdint.h>

#include "Types.h"
#include "Config.h"

#include "items/Equipment.h"
#include "items/Inventory.h"


class Dungeon;
class Monster;


class Player {
public:
    void begin();


    // Retourne l'index du monstre tué,
    // ou -1 si aucun monstre n'est mort.
    int8_t update(
        const Dungeon& dungeon,
        Monster* monsters,
        uint8_t monsterCount
    );


    // Position

    Position getPosition() const;

    void setPosition(
        Position position
    );


    // Orientation

    Direction getDirection() const;


    // Vie

    uint8_t getHp() const;

    uint8_t getMaxHp() const;

    bool isAlive() const;

    void takeDamage(
        uint8_t damage
    );


    // Progression

    uint8_t getLevel() const;

    uint16_t getXp() const;

    uint16_t getXpToNextLevel() const;

    void addXp(
        uint8_t amount
    );


    // Combat

    bool isAttacking() const;

    uint8_t getAttackDamage() const;

    uint8_t getAttackCooldownFrames() const;


    // Inventaire

    bool addItem(
        const Item& item
    );

    uint8_t getInventoryCount() const;

    const Item& getInventoryItem(
        uint8_t index
    ) const;

    bool equipInventoryItem(
        uint8_t index
    );


    // Equipement

    void equip(
        const Item& item
    );

    const Equipment& getEquipment() const;


private:

    Position position { 2, 2 };

    Direction direction =
        Direction::DOWN;


    // Vie / progression

    uint8_t hp = PLAYER_BASE_HP;

    uint8_t maxHp = PLAYER_BASE_HP;

    uint8_t level = 1;

    uint16_t xp = 0;


    // Combat

    uint8_t attackTimer = 0;

    uint8_t attackCooldown = 0;


    // Objets

    Inventory inventory;

    Equipment equipment;


    // Méthodes internes

    void tryMove(
        int8_t dx,
        int8_t dy,
        const Dungeon& dungeon,
        const Monster* monsters,
        uint8_t monsterCount
    );

    int8_t attack(
        Monster* monsters,
        uint8_t monsterCount
    );

    void levelUp();
};
