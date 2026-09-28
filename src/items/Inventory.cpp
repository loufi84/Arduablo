#include "items/Inventory.h"

void Inventory::clear() {
    count = 0;

    for (uint8_t i = 0; i < CAPACITY; ++i) {
        items[i] = Item {};
    }
}

bool Inventory::add(const Item& item) {
    if (!item.isValid()) {
        return false;
    }

    if (isFull()) {
        return false;
    }

    items[count] = item;
    ++count;

    return true;
}

bool Inventory::remove(uint8_t index, Item& removedItem) {
    if (index >= count) {
        return false;
    }

    removedItem = items[index];

    // Compactage du tableau
    for (uint8_t i = index; i +1 < count; ++i) {
        items[i] = items[i + 1];
    }

    --count;

    // Nettoyage de l'ancien dernier slot
    items[count] = Item {};

    return true;
}

const Item& Inventory::get(uint8_t index) const {
    return items[index];
}

uint8_t Inventory::getCount() const {
    return count;
}

bool Inventory::isFull() const {
    return count >= CAPACITY;
}

bool Inventory::isEmpty() const {
    return count == 0;
}
