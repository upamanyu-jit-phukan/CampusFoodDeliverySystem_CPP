#ifndef DELIVERY_SYSTEM_H
#define DELIVERY_SYSTEM_H

#include "Student.h"
#include "Admin.h"
#include "Menu.h"
#include "Order.h"
#include <string>
#include <vector>

class DeliverySystem {
private:
    std::vector<Student> students;
    Admin admin;
    Menu menu;
    std::vector<Order> orders;
public:
    DeliverySystem();
    void run();
    void loadStudents();
    void saveStudents();
    void loadOrders();
    void saveOrders();
    void loadMenu();
    void studentPortal();
    void adminPortal();
    Student* findStudentByID(const std::string& scholarID);
};

#endif
