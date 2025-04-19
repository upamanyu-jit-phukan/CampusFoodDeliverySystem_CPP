#ifndef ORDEREDITEM_H
#define ORDEREDITEM_H

#include "../include/FoodItem.h"

class OrderedItem {
public:
    FoodItem item;
    int quantity;

    float totalCost() const;
    void display() const;
};

#endif
