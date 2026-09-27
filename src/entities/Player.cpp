#include <Arduboy2.h>

#include "entities/Player.h"
#include "entities/Monster.h"
#include "world/Dungeon.h"

extern Arduboy2 arduboy;

void Player::begin() {
    position = { 2, 2};
}

void Player::update(
    const Dungeon& dungeon,
    Monster* monsters,
    uint8_t monsterCount
) {
    if (arduboy.justPressed(LEFT_BUTTON)) {
        direction = Direction::LEFT;
        tryMove(-1, 0, dungeon, monsters, monsterCount);
    }

    if (arduboy.justPressed(RIGHT_BUTTON)) {
        direction = Direction::RIGHT;
        tryMove(1, 0, dungeon, monsters, monsterCount);
    }

    if (arduboy.justPressed(UP_BUTTON)) {
        direction = Direction::UP;
        tryMove(0, -1, dungeon, monsters, monsterCount);
    }

    if (arduboy.justPressed(DOWN_BUTTON)) {
        direction = Direction::DOWN;
        tryMove(0, 1, dungeon, monsters, monsterCount);
    }

    if (arduboy.justPressed(A_BUTTON)) {
        attack(monsters, monsterCount);
    }
}

void Player::tryMove(int8_t dx, int8_t dy, const Dungeon& dungeon, const Monster* monsters, uint8_t monsterCount) {
    const int8_t newX = position.x + dx;
    const int8_t newY = position.y + dy;

    if (newX < 0 || newY < 0) {
        return;
    }

    if (newX >= Dungeon::MAP_WIDTH || newY >= Dungeon::MAP_HEIGHT) {
        return;
    }

    for (uint8_t i = 0; i < monsterCount; ++i) {
        if (!monsters[i].isAlive()) {
            continue;
        }

    const Position monsterPos = monsters[i].getPosition();

    if (monsterPos.x == newX && monsterPos.y == newY) {
        return;
    }
}

    if (dungeon.isWalkable(newX, newY)) {
        position.x = newX;
        position.y = newY;
    }
}

Position Player::getPosition() const {
    return position;
}

Direction Player::getDirection() const {
    return direction;
}

void Player::attack(Monster* monsters, uint8_t monsterCount) {
    int8_t targetX = position.x;
    int8_t targetY = position.y;

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
}