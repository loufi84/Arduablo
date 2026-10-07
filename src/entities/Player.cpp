#include <Arduboy2.h>

#include "Config.h"

#include "entities/Player.h"
#include "entities/Monster.h"

#include "world/Dungeon.h"


extern Arduboy2 arduboy;


void Player::begin() {
    position = { 2, 2 };

    direction =
        Direction::DOWN;


    // Progression
    level = 1;
    xp = 0;

    maxHp =
        PLAYER_BASE_HP;

    hp =
        maxHp;


    // Combat
    attackTimer = 0;
    attackCooldown = 0;


    // Objets
    inventory.clear();
    equipment.clear();
}


int8_t Player::update(
    const Dungeon& dungeon,
    Monster* monsters,
    uint8_t monsterCount
) {
    // Timers

    if (attackTimer > 0) {
        --attackTimer;
    }

    if (attackCooldown > 0) {
        --attackCooldown;
    }


    // Déplacement

    if (
        arduboy.justPressed(LEFT_BUTTON)
        ||
        (
            arduboy.pressed(LEFT_BUTTON)
            &&
            arduboy.everyXFrames(
                MOVE_REPEAT_FRAMES
            )
        )
    ) {
        direction =
            Direction::LEFT;

        tryMove(
            -1,
            0,
            dungeon,
            monsters,
            monsterCount
        );
    }

    else if (
        arduboy.justPressed(RIGHT_BUTTON)
        ||
        (
            arduboy.pressed(RIGHT_BUTTON)
            &&
            arduboy.everyXFrames(
                MOVE_REPEAT_FRAMES
            )
        )
    ) {
        direction =
            Direction::RIGHT;

        tryMove(
            1,
            0,
            dungeon,
            monsters,
            monsterCount
        );
    }

    else if (
        arduboy.justPressed(UP_BUTTON)
        ||
        (
            arduboy.pressed(UP_BUTTON)
            &&
            arduboy.everyXFrames(
                MOVE_REPEAT_FRAMES
            )
        )
    ) {
        direction =
            Direction::UP;

        tryMove(
            0,
            -1,
            dungeon,
            monsters,
            monsterCount
        );
    }

    else if (
        arduboy.justPressed(DOWN_BUTTON)
        ||
        (
            arduboy.pressed(DOWN_BUTTON)
            &&
            arduboy.everyXFrames(
                MOVE_REPEAT_FRAMES
            )
        )
    ) {
        direction =
            Direction::DOWN;

        tryMove(
            0,
            1,
            dungeon,
            monsters,
            monsterCount
        );
    }


    // Attaque

    if (
        arduboy.pressed(A_BUTTON)
        &&
        attackCooldown == 0
    ) {
        attackCooldown =
            getAttackCooldownFrames();

        return attack(
            monsters,
            monsterCount
        );
    }


    return -1;
}


// Déplacement

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


    // Limites du monde
    if (
        newX < 0
        ||
        newY < 0
        ||
        newX >= Dungeon::MAP_WIDTH
        ||
        newY >= Dungeon::MAP_HEIGHT
    ) {
        return;
    }


    // Collision avec les murs
    if (
        !dungeon.isWalkable(
            static_cast<uint8_t>(newX),
            static_cast<uint8_t>(newY)
        )
    ) {
        return;
    }


    // Collision avec les monstres
    for (
        uint8_t i = 0;
        i < monsterCount;
        ++i
    ) {
        if (!monsters[i].isAlive()) {
            continue;
        }

        const Position monsterPos =
            monsters[i].getPosition();

        if (
            monsterPos.x == newX
            &&
            monsterPos.y == newY
        ) {
            return;
        }
    }


    position.x = newX;
    position.y = newY;
}


// Attaque

int8_t Player::attack(
    Monster* monsters,
    uint8_t monsterCount
) {
    attackTimer =
        ATTACK_ANIMATION_FRAMES;


    int8_t targetX =
        position.x;

    int8_t targetY =
        position.y;


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


    for (
        uint8_t i = 0;
        i < monsterCount;
        ++i
    ) {
        if (!monsters[i].isAlive()) {
            continue;
        }


        const Position monsterPos =
            monsters[i].getPosition();


        if (
            monsterPos.x == targetX
            &&
            monsterPos.y == targetY
        ) {
            const bool killed =
                monsters[i].takeDamage(
                    getAttackDamage()
                );


            if (killed) {
                return
                    static_cast<int8_t>(i);
            }


            return -1;
        }
    }


    return -1;
}


// Position

Position Player::getPosition() const {
    return position;
}


void Player::setPosition(
    Position newPosition
) {
    position =
        newPosition;
}


// Orientation

Direction Player::getDirection() const {
    return direction;
}


// Vie

uint8_t Player::getHp() const {
    return hp;
}


uint8_t Player::getMaxHp() const {
    return maxHp;
}


bool Player::isAlive() const {
    return hp > 0;
}


void Player::takeDamage(
    uint8_t damage
) {
    if (damage >= hp) {
        hp = 0;
        return;
    }

    hp -= damage;
}


// XP / niveaux

uint8_t Player::getLevel() const {
    return level;
}


uint16_t Player::getXp() const {
    return xp;
}


uint16_t Player::getXpToNextLevel() const {
    return
        XP_BASE_REQUIREMENT
        +
        static_cast<uint16_t>(
            level - 1
        )
        *
        XP_PER_LEVEL;
}


void Player::addXp(
    uint8_t amount
) {
    xp += amount;


    while (
        xp >= getXpToNextLevel()
    ) {
        const uint16_t requiredXp =
            getXpToNextLevel();

        xp -= requiredXp;

        levelUp();
    }
}


void Player::levelUp() {
    ++level;

    maxHp +=
        HP_PER_LEVEL;


    // Full heal au level-up.
    hp =
        maxHp;
}


// Combat

bool Player::isAttacking() const {
    return
        attackTimer > 0;
}


uint8_t Player::getAttackDamage() const {
    // 1 point de dégâts naturel
    // + dégâts de l'arme.
    return
        1
        +
        equipment.getWeaponDamage();
}


uint8_t Player::getAttackCooldownFrames() const {
    const Item& weapon =
        equipment.getWeapon();


    if (!weapon.isValid()) {
        return
            FIST_ATTACK_COOLDOWN;
    }


    switch (weapon.type) {

        case ItemType::DAGGER:
            return
                DAGGER_ATTACK_COOLDOWN;

        case ItemType::SWORD:
            return
                SWORD_ATTACK_COOLDOWN;

        case ItemType::AXE:
            return
                AXE_ATTACK_COOLDOWN;

        default:
            return
                FIST_ATTACK_COOLDOWN;
    }
}

// Inventaire

bool Player::addItem(
    const Item& item
) {
    return
        inventory.add(item);
}


uint8_t Player::getInventoryCount() const {
    return
        inventory.getCount();
}


const Item& Player::getInventoryItem(
    uint8_t index
) const {
    return
        inventory.get(index);
}


bool Player::equipInventoryItem(
    uint8_t index
) {
    if (
        index >= inventory.getCount()
    ) {
        return false;
    }


    const Item& selected =
        inventory.get(index);


    if (
        selected.type != ItemType::DAGGER
        &&
        selected.type != ItemType::SWORD
        &&
        selected.type != ItemType::AXE
    ) {
        return false;
    }


    const Item oldWeapon =
        equipment.getWeapon();


    Item newWeapon;


    if (
        !inventory.remove(
            index,
            newWeapon
        )
    ) {
        return false;
    }


    equipment.equipWeapon(
        newWeapon
    );


    if (oldWeapon.isValid()) {
        inventory.add(
            oldWeapon
        );
    }


    return true;
}


// Equipement

void Player::equip(
    const Item& item
) {
    equipment.equipWeapon(
        item
    );
}


const Equipment& Player::getEquipment() const {
    return equipment;
}
