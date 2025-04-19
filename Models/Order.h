#ifndef ORDER_H
#define ORDER_H
#include <string>
#include <iostream>
using namespace std;

class Order {
public:
    string scholarID;
    string foodItem;
    int quantity;
    string status;

    void display() {
        cout << "Scholar ID: " << scholarID
             << " | Item: " << foodItem
             << " | Qty: " << quantity
             << " | Status: " << status << '\n';
    }
};

#endif
