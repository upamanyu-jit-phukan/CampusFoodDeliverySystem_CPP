#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>

#include "../include/DeliverySystem.h"
#include "../include/Order.h"

using namespace std;

string generateRandomPassword(int length) {
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string password = "";
    srand(time(0)); 
    for (int i = 0; i < length; ++i) {
        password += chars[rand() % chars.length()];
    }
    return password;
}

void DeliverySystem::registerCustomer(const string& scholarID) {
    if (scholarIDExists(scholarID)) {
        cout << "\nScholarID already exists!\n";
        return;
    }

    string password = generateRandomPassword(10);
    ofstream outFile("../data/students.txt", ios::app); // Append mode
    if (outFile.is_open()) {
        outFile << scholarID << "," << password << ",false\n";
        outFile.close();
        cout << "\nCustomer registered.\n";
    } else {
        cerr << "Error: Could not open students.txt for writing.";
    }
}

void DeliverySystem::saveOrdersToFile() {
    ofstream fout("../data/orders.txt");
    for (const auto& order : orders) {
        fout << "OrderID:" << order.orderId << '\n';
        fout << "ScholarID:" << order.scholarID << '\n';
        fout << "Phone:" << order.phoneNumber << '\n';
        fout << "Hostel:" << order.hostelNumber << '\n';
        fout << "Status:" << order.status << '\n';
        fout << "Time:" << order.timestamp << '\n';
        fout << "Items:\n";
        for (const auto& item : order.items) {
            fout << item.item.id << "," << item.quantity << '\n';
        }
        fout << "END\n";
    }
    fout.close();
}

void DeliverySystem::loadOrdersFromFile() {
    ifstream fin("../data/orders.txt");
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
                stringstream ss(line);
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
    ifstream fin("../data/students.txt");
    if (!fin) {
        cout << "Error: 'students.txt' not found.\n";
        return;
    }

    studentLoginData.clear();
    studentSignUp.clear();

    string line;
    while (getline(fin, line)) {
        size_t comma1 = line.find(',');
        size_t comma2 = line.find(',', comma1 + 1);
        if (comma1 != string::npos && comma2 != string::npos) {
            string scholarID = line.substr(0, comma1);
            string password = line.substr(comma1 + 1, comma2 - comma1 - 1);
            string signedUp = line.substr(comma2 + 1);
            studentLoginData[scholarID] = password;
            studentSignUp[scholarID] = signedUp;
        }
    }
    fin.close();
}

bool DeliverySystem::validateStudentLogin(const string& scholarID, const string& password) {
    auto it = studentLoginData.find(scholarID);
    if (it != studentLoginData.end() && it->second == password) {
        if (studentSignUp[scholarID] == "false") {
            cout << "\nFirst-time login detected. Please set a new password: ";
            string newPassword;
            cin >> newPassword;

            string confirmPassword;
            do {
                cout << "Confirm Password: ";
                cin >> confirmPassword;
                if (newPassword != confirmPassword) {
                    cout << "Passwords do not match. Try again.\n";
                }
            } while (newPassword != confirmPassword);

            studentLoginData[scholarID] = newPassword;
            studentSignUp[scholarID] = "true";

            ofstream fout("../data/students.txt");
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
    ifstream inFile("../data/students.txt");
    string line;
    while (getline(inFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            string existingID = line.substr(0, commaPos);
            if (existingID == scholarID) {
                return true;
            }
        }
    }
    return false;
}

void DeliverySystem::studentPortal() {
    string scholarID, password;
    cout << "\nEnter Scholar ID: ";
    cin >> scholarID;
    cout << "Enter Password: ";
    cin >> password;

    if (!validateStudentLogin(scholarID, password)) {
        cout << "Invalid Scholar ID or Password.\n";
        return;
    }

    cout << "Login successful.\n";

    int choice;
    do {
        cout << "\n====== Student Portal ======\n";
        cout << "1. View Menu\n2. Place Order\n3. View My Orders\n4. Cancel Order\n5. Logout\nEnter choice: ";
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
        cout << "3. Clear All Orders\n";
        cout << "4. Register a New Customer\n";
        cout << "5. Logout\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewOrders();
                break;

            case 2: {
                int orderId;
                string newStatus;
                cout << "Enter Order ID to update: ";
                cin >> orderId;
                cin.ignore(); // Clear newline from input buffer
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
                    orders.clear();
                    saveOrdersToFile();
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
                break;
            }

            case 5:
                cout << "Returning to main menu...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

void DeliverySystem::run() {
    menu.loadFromFile();         
    loadOrdersFromFile();        
    loadStudentLoginData();     

    int choice;
    do {
        cout << "\n====== Campus Food Delivery System ======\n";
        cout << "1. Student Portal\n";
        cout << "2. Admin Portal\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                studentPortal();
                break;

            case 2:
                adminPortal();
                break;

            case 3:
                cout << "\nThank you! Goodbye.\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 3);
}
