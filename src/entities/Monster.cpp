#include "Config.h"
#include "entities/Monster.h"

void Monster::begin() {

}

void Monster::update() {

}

void Monster::spawn(int8_t x, int8_t y, uint8_t hpValue) {
    position = { x, y };
    hp = hpValue;
    alive = true;
}

bool Monster::isAlive() const {
    return alive;
}

Position Monster::getPosition() const {
    return position;
}

uint8_t Monster::getHp() const {
    return hp;
}

bool Monster::takeDamage(uint8_t damage) {
    if (!alive) {
        return false;
    }

    if (damage >= hp) {
        hp = 0;
        alive = false;

        return true;
    }

    hp -= damage;

    return false;
}

void Monster::setPosition(int8_t x, int8_t y) {
    position = { x,y };
}

void Monster::tickAnimation() {
    if (attackTimer > 0) {
        --attackTimer;
    }
}

void Monster::startAttack(Direction direction) {
    attackDirection = direction;
    attackTimer = ATTACK_ANIMATION_FRAMES;
}

bool Monster::isAttacking() const {
    return attackTimer > 0;
}

Direction Monster::getAttackDirection() const {
    return attackDirection;
}
