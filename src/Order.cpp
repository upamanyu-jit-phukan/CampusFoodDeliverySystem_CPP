#include <bits/stdc++.h>
#include "../include/DeliverySystem.h"
#include "../include/Order.h"
using namespace std;

void Order::display() const {
    cout << "\nOrder ID: " << orderId
              << "\nScholarID: " << scholarID
              << "\nPhone: " << phoneNumber
              << "\nHostel: " << hostelNumber
              << "\nStatus: " << status
              << "\nTime: " << ctime(&timestamp)
              << "Items:\n";

    cout << left << setw(5) << "ID"
              << setw(20) << "Item"
              << setw(8) << "Qty"
              << "Total\n";

    float total = 0;
    for (const auto& ordered : items) {
        ordered.display();
        total += ordered.totalCost();
    }

    cout << "------------------------------\nTotal: Rs. " << total << endl;
}

void DeliverySystem::viewOrders() {
    if (orders.empty()) {
        cout << "\nNo orders!\n";
        return;
    }

    for (const auto& order : orders) {
        order.display();
    }
}

void DeliverySystem::cancelOrder(const string& scholarID) {
    int orderId;
    cout << "Enter Order ID to cancel: ";
    cin >> orderId;

    for (auto& order : orders) {
        if (order.scholarID == scholarID && order.orderId == orderId && order.status == "Pending") {
            order.status = "Cancelled";
            saveOrdersToFile();
            cout << "Order " << orderId << " has been cancelled.\n";
            return;
        }
    }
    cout << "Order not found or cannot be cancelled.\n";
}

void DeliverySystem::viewMyOrders(const string& scholarID) {
    bool found = false;
    for (const auto& order : orders) {
        if (order.scholarID == scholarID) {
            order.display();
            found = true;
        }
    }
    if (!found) {
        cout << "No orders found for " << scholarID << ".\n";
    }
}

void DeliverySystem::placeOrder(const string& scholarID) {
    vector<OrderedItem> orderedItems;
    float totalCost = 0;
    int foodId = -1, quantity;
    string phoneNumber, hostelNumber;

    cout << "\n--- Place Order ---\n";

    cout << "\nEnter 0 as Food ID when you're done ordering.";

    while (foodId!=0) {
        cout << "\nEnter Food ID to add to order (0 to finish): ";
        cin >> foodId;

        if (foodId == 0) break;

        FoodItem* item = menu.findItem(foodId);
        if (!item) {
            cout << "Food item not found. Please try again.\n";
            continue;
        }

        cout << "Enter Quantity: ";
        cin >> quantity;

        if (quantity <= 0) {
            cout << "Quantity must be positive.\n";
            continue;
        }

        OrderedItem orderedItem = { *item, quantity };
        totalCost += orderedItem.totalCost();
        orderedItems.push_back(orderedItem);

        cout << "Added " << quantity << " x " << item->name << " to your order.\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    }

    if (orderedItems.empty()) {
        cout << "No items added. Order not placed.\n";
        return;
    }
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter your phone number:";
    getline(cin, phoneNumber); 
    
    cout << "\nEnter your hostel number:";
    getline(cin, hostelNumber);
    
    Order newOrder;
    newOrder.orderId = nextOrderId++;
    newOrder.scholarID = scholarID;
    newOrder.timestamp = time(0);
    newOrder.items = orderedItems;
    newOrder.status = "Pending";
    newOrder.hostelNumber = hostelNumber;
    newOrder.phoneNumber = phoneNumber;

    orders.push_back(newOrder);
    saveOrdersToFile();

    cout << "\nOrder placed successfully! Total Cost: Rs. " << totalCost << endl;
}