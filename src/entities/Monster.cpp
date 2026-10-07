#include "Config.h"

#include "entities/Monster.h"
#include "data/MonsterData.h"


void Monster::spawn(
    int8_t x,
    int8_t y,
    MonsterType monsterType,
    uint8_t dungeonDepth
) {
    position.x = x;
    position.y = y;

    type =
        monsterType;

    depth =
        dungeonDepth;


    const MonsterDefinition definition =
        getMonsterDefinition(type);


    hp =
        definition.baseHp
        +
        depth / 3;


    alive = true;

    attackTimer = 0;

    attackDirection =
        Direction::DOWN;
}


bool Monster::isAlive() const {
    return alive;
}


Position Monster::getPosition() const {
    return position;
}


void Monster::setPosition(
    int8_t x,
    int8_t y
) {
    position.x = x;
    position.y = y;
}


MonsterType Monster::getType() const {
    return type;
}


uint8_t Monster::getHp() const {
    return hp;
}


uint8_t Monster::getDamage() const {
    const MonsterDefinition definition =
        getMonsterDefinition(type);


    return
        definition.baseDamage
        +
        depth / 5;
}


uint8_t Monster::getXpReward() const {
    const MonsterDefinition definition =
        getMonsterDefinition(type);


    return
        definition.xpReward;
}


uint8_t Monster::getActionDelay() const {
    const MonsterDefinition definition =
        getMonsterDefinition(type);


    return
        definition.actionDelay;
}


bool Monster::takeDamage(
    uint8_t damage
) {
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


void Monster::tickAnimation() {
    if (attackTimer > 0) {
        --attackTimer;
    }
}


void Monster::startAttack(
    Direction direction
) {
    attackDirection =
        direction;

    attackTimer =
        ATTACK_ANIMATION_FRAMES;
}


bool Monster::isAttacking() const {
    return
        attackTimer > 0;
}


Direction Monster::getAttackDirection() const {
    return attackDirection;
}
