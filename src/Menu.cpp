#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Menu.h"
using namespace std;

void Menu::loadFromFile() {
    ifstream fin("../data/menu.txt");
    if (!fin) {
        std::cout << "Error: 'menu.txt' not found.\n";
        return;
    }

    items.clear();
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, name, priceStr;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, priceStr);

        try {
            FoodItem item;
            item.id = std::stoi(idStr);
            item.name = name;
            item.price = std::stof(priceStr);
            items.push_back(item);
        } catch (...) {
            cout << "Invalid line in menu: " << line << endl;
        }
    }
    fin.close();
}

void Menu::display() const {
    cout << "\n=========== MENU ===========\n";
    cout << left << setw(5) << "ID" << setw(20) << "Item" << "Price\n";
    for (const auto& item : items) {
        item.display();
    }
}

FoodItem* Menu::findItem(int id) {
    for (auto& item : items) {
        if (item.id == id)
            return &item;
    }
    return nullptr;
}
