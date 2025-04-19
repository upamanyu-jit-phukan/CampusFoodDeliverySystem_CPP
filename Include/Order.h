#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <string>
#include <ctime>
#include "../include/OrderedItem.h"

using namespace std;

class Order {
public:
    int orderId;
    string scholarID;
    string phoneNumber;
    string hostelNumber;
    vector<OrderedItem> items;
    string status;
    time_t timestamp;

    void display() const;
};

#endif
