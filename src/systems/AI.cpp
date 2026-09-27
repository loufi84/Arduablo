#include "systems/AI.h"

#include "world/Dungeon.h"
#include "entities/Player.h"
#include "entities/Monster.h"

void AI::updateMonsters(
    const Dungeon& dungeon,
    Player& player,
    Monster* monsters,
    uint8_t monsterCount
) {
    for (uint8_t i = 0; i < monsterCount; ++i) {
        if (!monsters[i].isAlive()) {
            continue;
        } 

        updateZombie(
            dungeon,
            player,
            monsters[i],
            monsters,
            monsterCount
        );
    }
}

void AI::updateZombie(
    const Dungeon& dungeon,
    Player& player,
    Monster& monster,
    const Monster* monsters,
    uint8_t monsterCount
) {
    const Position monsterPos = monster.getPosition();
    const Position playerPos = player.getPosition();

    const int8_t dx = playerPos.x - monsterPos.x;
    const int8_t dy = playerPos.y - monsterPos.y;

    // Joueur adjacent : attaque
    if (
        (dx == 1 && dy == 0) ||
        (dx == -1 && dy == 0) ||
        (dx == 0 && dy == 1) ||
        (dx == 0 && dy == -1)
    ) {
        Direction attackDirection;

        if (dx > 0) {
            attackDirection = Direction::RIGHT;
        }
        else if (dx < 0) {
            attackDirection = Direction::LEFT;
        }
        else if (dy > 0) {
            attackDirection = Direction::DOWN;
        }
        else {
            attackDirection = Direction::UP;
        }

        monster.startAttack(attackDirection);
        player.takeDamage(1);

        return;
    }

    int8_t newX = monsterPos.x;
    int8_t newY = monsterPos.y;

    // Le zombie privilégie l'axe où la distance est la plus grande
    if (dx > 0) {
        ++newX;
    }
    else if (dx < 0) {
        --newX;
    }
    else if (dy > 0) {
        ++newY;
    }
    else if (dy < 0) {
        --newY;
    }

    if (!dungeon.isWalkable(newX, newY)) {
        return;
    }

    if (newX == playerPos.x && newY == playerPos.y) {
        return;
    }

    if (isOccupied(
        newX,
        newY,
        monsters,
        monsterCount,
        &monster
    )) {
        return;
    }

    monster.setPosition(newX, newY);
}

bool AI::isOccupied(
    int8_t x,
    int8_t y,
    const Monster* monsters,
    uint8_t monsterCount,
    const Monster* ignoredMonster
) {
    for (uint8_t i = 0; i < monsterCount; ++i) {

        if (&monsters[i] == ignoredMonster) {
            continue;
        }

        if (!monsters[i].isAlive()) {
            continue;
        }

        const Position pos = monsters[i].getPosition();

        if (pos.x == x && pos.y == y) {
            return true;
        }
    }

    return false;
}
