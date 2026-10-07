#include "items/Equipment.h"

void Equipment::clear() {
    weapon = Item {};
}

void Equipment::equipWeapon(const Item& item) {
    if (
        item.type == ItemType::SWORD ||
        item.type == ItemType::AXE ||
        item.type == ItemType::DAGGER
    ) {
        weapon = item;
    }
}

uint8_t Equipment::getWeaponDamage() const {
    return weapon.damage;
}

const Item& Equipment::getWeapon() const {
    return weapon;
}
