#ifndef FOODITEM_H
#define FOODITEM_H

#include <string>
using namespace std;

class FoodItem {
public:
    int id;
    string name;
    float price;

    void display() const;
};

#endif
