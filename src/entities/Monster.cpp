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

void Monster::takeDamage(uint8_t damage) {
    if (!alive) {
        return;
    }

    if (damage >= hp) {
        hp = 0;
        alive = false;
        return;
    }

    hp -= damage;
}

void Monster::setPosition(int8_t x, int8_t y) {
    position = { x,y };
}
