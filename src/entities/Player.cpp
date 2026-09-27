#include <Arduboy2.h>

#include "Config.h"
#include "entities/Player.h"
#include "entities/Monster.h"
#include "world/Dungeon.h"

extern Arduboy2 arduboy;


void Player::begin() {
    position = { 2, 2 };

    direction = Direction::DOWN;

    maxHp = 5;
    hp = maxHp;

    attackTimer = 0;

    equipment.clear();
}


int8_t Player::update(
    const Dungeon& dungeon,
    Monster* monsters,
    uint8_t monsterCount
) {
    // Animation d'attaque
    if (attackTimer > 0) {
        --attackTimer;
    }


    // --------------------
    // Déplacement
    // --------------------

    if (
        arduboy.justPressed(LEFT_BUTTON) ||
        (
            arduboy.pressed(LEFT_BUTTON) &&
            arduboy.everyXFrames(MOVE_REPEAT_FRAMES)
        )
    ) {
        direction = Direction::LEFT;

        tryMove(
            -1,
            0,
            dungeon,
            monsters,
            monsterCount
        );
    }

    else if (
        arduboy.justPressed(RIGHT_BUTTON) ||
        (
            arduboy.pressed(RIGHT_BUTTON) &&
            arduboy.everyXFrames(MOVE_REPEAT_FRAMES)
        )
    ) {
        direction = Direction::RIGHT;

        tryMove(
            1,
            0,
            dungeon,
            monsters,
            monsterCount
        );
    }

    else if (
        arduboy.justPressed(UP_BUTTON) ||
        (
            arduboy.pressed(UP_BUTTON) &&
            arduboy.everyXFrames(MOVE_REPEAT_FRAMES)
        )
    ) {
        direction = Direction::UP;

        tryMove(
            0,
            -1,
            dungeon,
            monsters,
            monsterCount
        );
    }

    else if (
        arduboy.justPressed(DOWN_BUTTON) ||
        (
            arduboy.pressed(DOWN_BUTTON) &&
            arduboy.everyXFrames(MOVE_REPEAT_FRAMES)
        )
    ) {
        direction = Direction::DOWN;

        tryMove(
            0,
            1,
            dungeon,
            monsters,
            monsterCount
        );
    }


    // --------------------
    // Attaque
    // --------------------

    // Pour l'instant : uniquement sur pression.
    // Pas de maintien automatique afin d'éviter
    // la mitraillette à épée xD
    if (arduboy.justPressed(A_BUTTON)) {
        return attack(
            monsters,
            monsterCount
        );
    }


    return -1;
}


void Player::tryMove(
    int8_t dx,
    int8_t dy,
    const Dungeon& dungeon,
    const Monster* monsters,
    uint8_t monsterCount
) {
    const int8_t newX =
        position.x + dx;

    const int8_t newY =
        position.y + dy;


    // Limites du donjon
    if (
        newX < 0 ||
        newY < 0 ||
        newX >= Dungeon::MAP_WIDTH ||
        newY >= Dungeon::MAP_HEIGHT
    ) {
        return;
    }


    // Collision avec la map
    if (!dungeon.isWalkable(
        static_cast<uint8_t>(newX),
        static_cast<uint8_t>(newY)
    )) {
        return;
    }


    // Collision avec les monstres
    for (uint8_t i = 0; i < monsterCount; ++i) {
        if (!monsters[i].isAlive()) {
            continue;
        }

        const Position monsterPos =
            monsters[i].getPosition();

        if (
            monsterPos.x == newX &&
            monsterPos.y == newY
        ) {
            return;
        }
    }


    position.x = newX;
    position.y = newY;
}


int8_t Player::attack(
    Monster* monsters,
    uint8_t monsterCount
) {
    // Déclenche l'animation même si on frappe dans le vide
    attackTimer = ATTACK_ANIMATION_FRAMES;


    int8_t targetX = position.x;
    int8_t targetY = position.y;


    // La cible est la case immédiatement devant le joueur
    switch (direction) {

        case Direction::UP:
            --targetY;
            break;

        case Direction::DOWN:
            ++targetY;
            break;

        case Direction::LEFT:
            --targetX;
            break;

        case Direction::RIGHT:
            ++targetX;
            break;
    }


    for (uint8_t i = 0; i < monsterCount; ++i) {
        if (!monsters[i].isAlive()) {
            continue;
        }


        const Position monsterPos =
            monsters[i].getPosition();


        if (
            monsterPos.x == targetX &&
            monsterPos.y == targetY
        ) {
            const bool killed =
                monsters[i].takeDamage(
                    getAttackDamage()
                );


            // Game pourra générer le loot
            // à l'emplacement de ce monstre.
            if (killed) {
                return static_cast<int8_t>(i);
            }


            return -1;
        }
    }


    return -1;
}


// --------------------
// Position
// --------------------

Position Player::getPosition() const {
    return position;
}


void Player::setPosition(Position newPosition) {
    position = newPosition;
}


// --------------------
// Orientation
// --------------------

Direction Player::getDirection() const {
    return direction;
}


// --------------------
// Vie
// --------------------

uint8_t Player::getHp() const {
    return hp;
}


uint8_t Player::getMaxHp() const {
    return maxHp;
}


bool Player::isAlive() const {
    return hp > 0;
}


void Player::takeDamage(uint8_t damage) {
    if (damage >= hp) {
        hp = 0;
        return;
    }

    hp -= damage;
}


// --------------------
// Combat
// --------------------

bool Player::isAttacking() const {
    return attackTimer > 0;
}


uint8_t Player::getAttackDamage() const {
    // 1 point de dégâts de base à mains nues
    // + dégâts apportés par l'arme.
    return 1 + equipment.getWeaponDamage();
}


// --------------------
// Equipement
// --------------------

void Player::equip(const Item& item) {
    equipment.equipWeapon(item);
}


const Equipment& Player::getEquipment() const {
    return equipment;
}
