#pragma once

#include <stdint.h>

#include "items/Item.h"

class Equipment {
public:
    void clear();

    void equipWeapon(const Item& item);

    uint8_t getWeaponDamage() const;
    const Item& getWeapon() const;

private:
    Item weapon;
};
