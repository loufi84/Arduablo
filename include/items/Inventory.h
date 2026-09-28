#pragma once

#include <stdint.h>

#include "items/Item.h"

class Inventory {
    public:
        static constexpr uint8_t CAPACITY = 6;

        void clear();

        bool add(const Item& item);

        bool remove(uint8_t index, Item& removedItem);

        const Item& get(uint8_t index) const;

        uint8_t getCount() const;

        bool isFull() const;
        bool isEmpty() const;

    private:
        Item items[CAPACITY];
        uint8_t count = 0;
};
