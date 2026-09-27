#pragma once

#include <stdint.h>

constexpr uint8_t FPS = 30;
constexpr uint8_t TILE_SIZE = 8;

constexpr uint8_t MAP_WIDTH  = 32;
constexpr uint8_t MAP_HEIGHT = 32;

constexpr uint8_t VIEW_WIDTH  = 16;
constexpr uint8_t VIEW_HEIGHT = 7;
constexpr uint8_t MOVE_REPEAT_FRAMES = 4;
constexpr uint8_t ATTACK_ANIMATION_FRAMES = 4;

constexpr uint8_t MAX_MONSTERS = 8;
constexpr uint8_t MAX_ITEMS = 8;
constexpr uint8_t INVENTORY_SIZE = 8;

constexpr uint8_t MIN_ROOMS = 5;
constexpr uint8_t MAX_ROOMS = 7;

constexpr uint8_t MIN_ROOM_SIZE = 4;
constexpr uint8_t MAX_ROOM_SIZE = 8;

constexpr uint8_t ROOM_GENERATION_ATTEMPTS = 40;
