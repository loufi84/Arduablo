#pragma once

#include <stdint.h>


// Jeu / affichage
constexpr uint8_t FPS = 30;
constexpr uint8_t TILE_SIZE = 16;
constexpr uint8_t MAP_WIDTH  = 32;
constexpr uint8_t MAP_HEIGHT = 32;
constexpr uint8_t VIEW_WIDTH  = 8;
constexpr uint8_t VIEW_HEIGHT = 3;


// Entités
constexpr uint8_t MAX_MONSTERS = 8;
constexpr uint8_t MAX_ITEMS = 8;


// Donjon
constexpr uint8_t MIN_ROOMS = 5;
constexpr uint8_t MAX_ROOMS = 7;
constexpr uint8_t MIN_ROOM_SIZE = 4;
constexpr uint8_t MAX_ROOM_SIZE = 8;
constexpr uint8_t ROOM_GENERATION_ATTEMPTS = 40;


// Déplacement
constexpr uint8_t MOVE_REPEAT_FRAMES = 4;


// Combat
constexpr uint8_t ATTACK_ANIMATION_FRAMES = 4;
constexpr uint8_t FIST_ATTACK_COOLDOWN   = 10;
constexpr uint8_t DAGGER_ATTACK_COOLDOWN = 5;
constexpr uint8_t SWORD_ATTACK_COOLDOWN  = 8;
constexpr uint8_t AXE_ATTACK_COOLDOWN    = 12;


// Progression joueur
constexpr uint8_t PLAYER_BASE_HP = 8;
constexpr uint8_t HP_PER_LEVEL = 2;
constexpr uint8_t XP_BASE_REQUIREMENT = 6;
constexpr uint8_t XP_PER_LEVEL = 4;
