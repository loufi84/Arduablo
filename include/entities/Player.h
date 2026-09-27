#pragma once

#include <stdint.h>

#include "Types.h"
#include "items/Equipment.h"

class Dungeon;
class Monster;

class Player {
public:
    void begin();

    /**
     * Met à jour le joueur.
     *
     * Retourne :
     *  - index du monstre tué
     *  - -1 si aucun monstre n'a été tué
     */
    int8_t update(
        const Dungeon& dungeon,
        Monster* monsters,
        uint8_t monsterCount
    );

    // Position
    Position getPosition() const;
    void setPosition(Position position);

    // Orientation
    Direction getDirection() const;

    // Vie
    uint8_t getHp() const;
    uint8_t getMaxHp() const;

    bool isAlive() const;
    void takeDamage(uint8_t damage);

    // Combat
    bool isAttacking() const;
    uint8_t getAttackDamage() const;

    // Equipement
    void equip(const Item& item);
    const Equipment& getEquipment() const;

private:
    Position position { 2, 2 };

    Direction direction = Direction::DOWN;

    uint8_t hp = 5;
    uint8_t maxHp = 5;

    uint8_t attackTimer = 0;

    Equipment equipment;

    void tryMove(
        int8_t dx,
        int8_t dy,
        const Dungeon& dungeon,
        const Monster* monsters,
        uint8_t monsterCount
    );

    /**
     * Retourne :
     *  - index du monstre tué
     *  - -1 sinon
     */
    int8_t attack(
        Monster* monsters,
        uint8_t monsterCount
    );
};
