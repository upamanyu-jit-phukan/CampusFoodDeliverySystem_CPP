#ifndef MENU_H
#define MENU_H

#include <vector>
#include "../include/FoodItem.h"

using namespace std;

class Menu {
public:
    vector<FoodItem> items;
    void loadFromFile();
    void display() const;
    FoodItem* findItem(int id);
};

#endif
