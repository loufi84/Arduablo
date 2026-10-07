#pragma once

#include <stdint.h>

enum class MonsterType : uint8_t {
    ZOMBIE = 0,
    SKELETON,
    BRUTE,
    BONE_WARDEN,
    ABYSS_LORD
};

struct MonsterDefinition {
    uint8_t baseHp;
    uint8_t baseDamage;
    uint8_t actionDelay;
    uint8_t xpReward;
};

MonsterDefinition getMonsterDefinition(MonsterType type);
