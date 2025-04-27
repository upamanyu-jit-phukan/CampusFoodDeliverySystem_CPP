#include <bits/stdc++.h>
#include "../include/OrderedItem.h"
using namespace std;

float OrderedItem::totalCost() const {
    return quantity * item.price;
}

void OrderedItem::display() const {
    cout << left << setw(5) << item.id
              << setw(20) << item.name
              << setw(8) << quantity
              << "Rs. " << totalCost() << endl;
}
