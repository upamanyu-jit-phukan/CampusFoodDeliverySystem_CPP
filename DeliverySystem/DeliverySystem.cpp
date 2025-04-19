#include "DeliverySystem.h"
#include "Student.h"
#include <iostream>
#include <fstream>
using namespace std;

DeliverySystem::DeliverySystem() {
    loadStudents();
    loadOrders();
    loadMenu();
}

void DeliverySystem::run() {
    int choice;
    do {
        cout << "\n1. Student Portal\n2. Admin Portal\n3. Exit\nEnter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: studentPortal(); break;
            case 2: adminPortal(); break;
            case 3: saveStudents(); saveOrders(); break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);
}

void DeliverySystem::loadStudents() {
    ifstream in("students.txt");
    Student s;
    while (in >> s.scholarID >> s.password >> s.hasChangedPassword) {
        students.push_back(s);
    }
    in.close();
}

void DeliverySystem::saveStudents() {
    ofstream out("students.txt");
    for (auto& s : students) {
        out << s.scholarID << ' ' << s.password << ' ' << s.hasChangedPassword << '\n';
    }
    out.close();
}

void DeliverySystem::loadOrders() {
    ifstream in("orders.txt");
    Order o;
    while (in >> o.scholarID >> o.foodItem >> o.quantity >> o.status) {
        orders.push_back(o);
    }
    in.close();
}

void DeliverySystem::saveOrders() {
    ofstream out("orders.txt");
    for (auto& o : orders) {
        out << o.scholarID << ' ' << o.foodItem << ' ' << o.quantity << ' ' << o.status << '\n';
    }
    out.close();
}

void DeliverySystem::loadMenu() {
    menu.loadFromFile("menu.txt");
}

Student* DeliverySystem::findStudentByID(const string& id) {
    for (auto& s : students) {
        if (s.scholarID == id) return &s;
    }
    return nullptr;
}

void DeliverySystem::studentPortal() {
    string id, pass;
    cout << "Enter Scholar ID: "; cin >> id;
    Student* student = findStudentByID(id);

    if (!student) {
        cout << "No student found.\n";
        return;
    }

    cout << "Enter Password: "; cin >> pass;
    if (pass != student->password) {
        cout << "Incorrect password.\n";
        return;
    }

    if (!student->hasChangedPassword) {
        string newPass;
        cout << "First time login. Change your password: ";
        cin >> newPass;
        student->password = newPass;
        student->hasChangedPassword = true;
        cout << "Password changed successfully.\n";
    }

    int choice;
    do {
        cout << "\n1. View Menu\n2. Place Order\n3. View My Orders\n4. Cancel Order\n5. Logout\nEnter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: menu.display(); break;
            case 2: {
                string item; int qty;
                cout << "Enter item name and quantity: ";
                cin >> item >> qty;
                orders.push_back(Order{id, item, qty, "Placed"});
                cout << "Order placed successfully.\n";
                break;
            }
            case 3:
                for (auto& o : orders) if (o.scholarID == id) o.display();
                break;
            case 4: {
                string item;
                cout << "Enter item to cancel: "; cin >> item;
                for (auto& o : orders) {
                    if (o.scholarID == id && o.foodItem == item && o.status == "Placed") {
                        o.status = "Cancelled";
                        cout << "Order cancelled.\n";
                        break;
                    }
                }
                break;
            }
        }
    } while (choice != 5);
}

void DeliverySystem::adminPortal() {
    string pass;
    cout << "Enter Admin Password: "; cin >> pass;
    if (pass != admin.password) {
        cout << "Access Denied.\n";
        return;
    }

    int choice;
    do {
        cout << "\n1. View All Orders\n2. Update Order Status\n3. View Menu\n4. Logout\nEnter choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                for (auto& o : orders) o.display();
                break;
            case 2: {
                string id, item, status;
                cout << "Enter Scholar ID and Item to update: "; cin >> id >> item;
                cout << "Enter new status: "; cin >> status;
                for (auto& o : orders) {
                    if (o.scholarID == id && o.foodItem == item) {
                        o.status = status;
                        cout << "Status updated.\n";
                        break;
                    }
                }
                break;
            }
            case 3:
                menu.display();
                break;
        }
    } while (choice != 4);
}
