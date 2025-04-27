#include <bits/stdc++.h>
#include "../include/FoodItem.h"
using namespace std;

void FoodItem::display() const {
    cout << left << setw(5) << id
              << setw(20) << name
              << "Rs. " << price << endl;
}
