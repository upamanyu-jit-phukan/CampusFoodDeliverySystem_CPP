#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <ctime>
#include <regex>
#include <sstream>
#include <unordered_map>

using namespace std;


string generateRandomPassword(int length) {
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string password = "";
    srand(time(0)); // Seed only once

    for (int i = 0; i < length; ++i) {
        password += chars[rand() % chars.length()];
    }

    return password;
}

class FoodItem {
public:
    int id;
    string name;
    float price;

    void display() const {
        cout << left << setw(5) << id << setw(20) << name << "Rs. " << price << endl;
    }
};

class OrderedItem {
public:
    FoodItem item;
    int quantity;

    float totalCost() const {
        return quantity * item.price;
    }

    void display() const {
        cout << left << setw(5) << item.id << setw(20) << item.name
             << setw(8) << quantity << "Rs. " << totalCost() << endl;
    }
};

class Order {
public:
    int orderId;
    string scholarID;
    string phoneNumber;
    string hostelNumber;
    vector<OrderedItem> items;
    string status;
    time_t timestamp;

    void display() const {
        cout << "\nOrder ID: " << orderId << "\nScholarID: " << scholarID
             << "\nPhone: " << phoneNumber << "\nHostel: " << hostelNumber
             << "\nStatus: " << status << "\nTime: " << ctime(&timestamp) << "Items:\n";
        cout << left << setw(5) << "ID" << setw(20) << "Item"
             << setw(8) << "Qty" << "Total\n";
        for (const auto& ordered : items) {
            ordered.display();
        }
        float total = 0;
        for (const auto& ordered : items) {
            total += ordered.totalCost();
        }
        cout << "------------------------------\nTotal: Rs. " << total << endl;
    }
};

class Menu {
public:
    vector<FoodItem> items;

    void loadFromFile() {
        ifstream fin("menu.txt");
        if (!fin) {
            cout << "Error: 'menu.txt' not found.\n";
            return;
        }

        items.clear();
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;

            size_t firstComma = line.find(',');
            size_t secondComma = line.find(',', firstComma + 1);

            if (firstComma != string::npos && secondComma != string::npos) {
                try {
                    FoodItem item;
                    item.id = stoi(line.substr(0, firstComma)); // string.substr(start_pos, length)
                    item.name = line.substr(firstComma + 1, secondComma - firstComma - 1);
                    item.price = stof(line.substr(secondComma + 1));
                    items.push_back(item);
                } catch (exception& e) {
                    cout << "Error reading line: " << line << "\nReason: " << e.what() << endl;
                }
            } else {
                cout << "Skipping badly formatted line: " << line << endl;
            }
        }
        fin.close();
    }

    void display() const {
        cout << "\n=========== MENU ===========\n";
        cout << left << setw(5) << "ID" << setw(20) << "Item" << "Price\n";
        for (const auto& item : items) {
            item.display();
        }
    }

    FoodItem* findItem(int id) {
        for (auto& item : items) {
            if (item.id == id) return &item;
        }
        return nullptr;
    }
};

class DeliverySystem {
private:
    Menu menu;
    vector<Order> orders;
    int nextOrderId = 1;
    unordered_map<string, string> studentLoginData;
    unordered_map<string, string> studentSignUp;

public:
    void run();
    void studentPortal();
    void adminPortal();

    void placeOrder(const string& name);
    void viewMyOrders(const string& scholarID);
    void cancelOrder(const string& scholarID);
    void viewOrders();

    void saveOrdersToFile();
    void loadOrdersFromFile();
    void loadStudentLoginData();
    bool validateStudentLogin(const string& scholarID, const string& password);
    void registerCustomer(const string& scholarID);
    bool scholarIDExists(const string& scholarID);
};

void DeliverySystem::registerCustomer(const string& scholarID) {
    if(scholarIDExists(scholarID)) {
        cout << "\nScholarID already exists!\n";
        return;
    }
    string password = generateRandomPassword(10);

    ofstream outFile("students.txt", ios::app); // Append mode
    if (outFile.is_open()) {
        outFile << scholarID << "," << password << ",false" << endl;
        outFile.close();
        cout << "\nCustomer registered.\n";
    } else {
        cerr << "Error: Could not open students.txt for writing.";
    }
}

void DeliverySystem::saveOrdersToFile() {
    ofstream fout("orders.txt");
    for (const auto& order : orders) {
        fout << "OrderID:" << order.orderId << '\n';
        fout << "ScholarID:" << order.scholarID << '\n';
        fout << "Phone:" << order.phoneNumber << '\n';
        fout << "Hostel:" << order.hostelNumber << '\n';
        fout << "Status:" << order.status << '\n';
        fout << "Time:" << order.timestamp << '\n';
        fout << "Items:" << '\n';
        for (const auto& item : order.items) {
            fout << item.item.id << "," << item.quantity << '\n';
        }
        fout << "END\n";
    }
    fout.close();
}

void DeliverySystem::loadOrdersFromFile() {
    ifstream fin("orders.txt");
    if (!fin) return;

    orders.clear();
    string line;
    Order order;
    while (getline(fin, line)) {
        if (line.find("OrderID:") == 0) {
            order = Order();
            order.orderId = stoi(line.substr(8));
            nextOrderId = max(nextOrderId, order.orderId + 1);
        } else if (line.find("ScholarID:") == 0) {
            order.scholarID = line.substr(10);
        } else if (line.find("Phone:") == 0) {
            order.phoneNumber = line.substr(6);
        } else if (line.find("Hostel:") == 0) {
            order.hostelNumber = line.substr(7);
        } else if (line.find("Status:") == 0) {
            order.status = line.substr(7);
        } else if (line.find("Time:") == 0) {
            order.timestamp = stol(line.substr(5));
        } else if (line == "Items:") {
            order.items.clear();
            while (getline(fin, line) && line != "END") {
                stringstream ss(line); // splits string into parts
                string idStr, qtyStr;
                getline(ss, idStr, ',');
                getline(ss, qtyStr);
                int id = stoi(idStr);
                int qty = stoi(qtyStr);
                FoodItem* f = menu.findItem(id);
                if (f) order.items.push_back({ *f, qty });
            }
            orders.push_back(order);
        }
    }
    fin.close();
}

void DeliverySystem::loadStudentLoginData() {
    ifstream fin("students.txt");
    if (!fin) {
        cout << "Error: 'students.txt' not found.\n";
        return;
    }

    studentLoginData.clear();
    studentSignUp.clear();

    try {
        string line;
        while (getline(fin, line)) {
            size_t comma1 = line.find(',');
            size_t comma2 = line.find(',', comma1 + 1);
            if (comma1 != string::npos) {
                string scholarID = line.substr(0, comma1);
                string password = line.substr(comma1 + 1, comma2 - comma1 - 1);
                string signedUp = line.substr(comma2 + 1);
                studentLoginData[scholarID] = password;
                studentSignUp[scholarID] = signedUp;
            } else {
                cerr << "Warning: Skipping badly formatted line -> " << line << endl;
            }
        }
    } catch (exception& e) {
        cerr << "Exception while reading file: " << e.what() << endl;
    }
}

bool DeliverySystem::validateStudentLogin(const string& scholarID, const string& password) {
    auto it = studentLoginData.find(scholarID);
    if (it != studentLoginData.end() && it->second == password) {
        if (studentSignUp[scholarID] == "false") {
            cout << "\nFirst-time login detected. Please set a new password: ";
            string newPassword;
            cin >> newPassword;
            
            string newPassword2;
            do {
                cout << "Confirm Password: ";
                cin >> newPassword2; 
                if (newPassword != newPassword2) {
                    cout << "Passwords do not match. Please try again.\n";
                }
            } while (newPassword2!=newPassword);

            studentLoginData[scholarID] = newPassword;
            studentSignUp[scholarID] = "true";
            
            ofstream fout("students.txt");
            for (const auto& entry : studentLoginData) {
                fout << entry.first << "," << entry.second << "," << studentSignUp[entry.first] << "\n";
            }
            fout.close();

            cout << "Password updated successfully.\n";
        }
        return true;
    }
    return false;
}

bool DeliverySystem::scholarIDExists(const string& scholarID) {
    ifstream inFile("students.txt");
    string line;
    
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            size_t commaPos = line.find(',');
            if (commaPos != string::npos) {
                string existingID = line.substr(0, commaPos);
                if (existingID == scholarID) {
                    inFile.close();
                    return true;
                }
            }
        }
        inFile.close();
    }
    return false;
}

void DeliverySystem::placeOrder(const string& scholarID) {
    int foodId, quantity;
    cout << "Enter Food ID to place an order: ";
    cin >> foodId;

    FoodItem* item = menu.findItem(foodId);
    if (!item) {
        cout << "Food item not found.\n";
        return;
    }

    cout << "Enter Quantity: ";
    cin >> quantity;

    OrderedItem orderedItem = { *item, quantity };
    Order newOrder;
    newOrder.orderId = nextOrderId++;
    newOrder.scholarID = scholarID;
    newOrder.timestamp = time(0); 
    newOrder.items.push_back(orderedItem);
    newOrder.status = "Pending";

    orders.push_back(newOrder);
    saveOrdersToFile();
    cout << "Order placed successfully.\n";
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
    cout << "Order not found or can't be cancelled.\n";
}

void DeliverySystem::viewOrders() {
    if(orders.empty()) {
        cout << "\nNo order!\n";
        return;
    } 
    for (const auto& order : orders) {
        order.display();
    }
}

void DeliverySystem::studentPortal() {
    string scholarID, password;
    cout << "\nEnter Scholar ID: ";
    cin >> scholarID;
    cout << "Enter Password: ";
    cin >> password;

    if (validateStudentLogin(scholarID, password)) {
        cout << "Login successful.\n";
    } else {
        cout << "Invalid Scholar ID or Password.\n";
        return;
    }

    int choice;
    do {
        cout << "\n====== Student Portal ======\n";
        cout << "1. View Menu\n2. Place Order\n3. View My Orders\n4. Cancel Order\n5. Back\nEnter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: menu.display(); break;
            case 2: placeOrder(scholarID); break;
            case 3: viewMyOrders(scholarID); break;
            case 4: cancelOrder(scholarID); break;
            case 5: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

void DeliverySystem::adminPortal() {
    string password;
    cout << "\nEnter Admin Password: ";
    cin >> password;

    if (password != "admin123") {
        cout << "Access Denied: Incorrect Password.\n";
        return;
    }

    int choice;
    do {
        cout << "\n====== Admin Portal ======\n";
        cout << "1. View All Orders\n";
        cout << "2. Update Order Status\n";
        cout << "3. Clear all orders\n";
        cout << "4. Register a New Customer\n";
        cout << "5. Back\nEnter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: viewOrders(); break;
            case 2: {
                int orderId;
                string newStatus;
                cout << "Enter Order ID to update: ";
                cin >> orderId;
                cin.ignore();
                cout << "Enter new status (e.g., Preparing, Delivered): ";
                getline(cin, newStatus);
                bool found = false;
                for (auto& order : orders) {
                    if (order.orderId == orderId) {
                        order.status = newStatus;
                        saveOrdersToFile();
                        cout << "Order status updated successfully!\n";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Order ID not found.\n";
                }
                break;
            }
            case 3: {
                char confirm;
                cout << "Are you sure you want to delete ALL orders? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    orders.clear();               // Clear all orders from memory
                    saveOrdersToFile();           // Save the empty list to the file
                    cout << "All orders have been cleared successfully.\n";
                } else {
                    cout << "Operation cancelled.\n";
                }
                break;
            }
            case 4: {
                cout << "Enter ScholarID of customer: ";
                string sID;
                cin >> sID;
                registerCustomer(sID);
                loadStudentLoginData();
            }
            case 5: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}


void DeliverySystem::run() {
    menu.loadFromFile();
    loadOrdersFromFile();
    loadStudentLoginData();
    int choice;
    do {
        cout << "\n====== Campus Food Delivery ======\n";
        cout << "1. Student Portal\n2. Admin Portal\n3. Exit\nEnter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: studentPortal(); break;
            case 2: adminPortal(); break;
            case 3: cout << "\nThank you! Goodbye.\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);
}

int main() {
    DeliverySystem system;
    system.run();
    return 0;
}