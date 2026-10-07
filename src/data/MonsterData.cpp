#include <Arduino.h>
#include <avr/pgmspace.h>

#include "data/MonsterData.h"

const MonsterDefinition MONSTER_DATA[] PROGMEM = {
    // HP, DMG, DELAY, XP
    { 3, 1, 10, 2},
    { 2, 1, 12, 3},
    { 7, 2, 16, 5}
};

MonsterDefinition getMonsterDefinition(MonsterType type) {
    MonsterDefinition definition;

    const uint8_t index = static_cast<uint8_t>(type);

    memcpy_P(&definition, &MONSTER_DATA[index], sizeof(MonsterDefinition));

    return definition;
}
