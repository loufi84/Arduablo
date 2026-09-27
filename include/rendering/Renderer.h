#pragma once

#include <stdint.h>

class Dungeon;
class Player;
class Monster;
class Camera;
class GroundItem;

class Renderer {
    public:
        void drawDungeon(const Dungeon& dungeon, const Camera& camera);
        void drawPlayer(const Player& player, const Camera& camera);
        void drawMonsters(const Monster* monsters, uint8_t monsterCount, const Camera& camera);
        void drawPlayerAttack(const Player& player, const Camera& camera);
        void drawMonsterAttack(const Monster* monsters, uint8_t monsterCount, const Camera& camera);
        void drawGroundItem(const GroundItem* items, uint8_t itemCount, const Camera& camera);
};
