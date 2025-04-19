#ifndef DELIVERYSYSTEM_H
#define DELIVERYSYSTEM_H

#include <unordered_map>
#include "../include/Menu.h"
#include "../include/Order.h"

using namespace std;

class DeliverySystem {
private:
    Menu menu;
    vector<Order> orders;
    int nextOrderId = 1;

    unordered_map<string, string> studentLoginData;
    unordered_map<string, string> studentSignUp;

    bool scholarIDExists(const string& scholarID);
    bool validateStudentLogin(const string& scholarID, const string& password);
    void saveOrdersToFile();
    void loadOrdersFromFile();
    void loadStudentLoginData();
    void registerCustomer(const string& scholarID);

public:
    void run();
    void studentPortal();
    void adminPortal();
    void placeOrder(const string& scholarID);
    void viewMyOrders(const string& scholarID);
    void cancelOrder(const string& scholarID);
    void viewOrders();
};

#endif
