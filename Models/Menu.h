#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

class Menu {
    map<string, int> items;
public:
    void loadFromFile(const string& filename) {
        ifstream in(filename);
        string name; int price;
        while (in >> name >> price) items[name] = price;
        in.close();
    }
    void display() {
        cout << "\n--- Menu ---\n";
        for (auto& item : items)
            cout << item.first << " - Rs. " << item.second << '\n';
    }
};

#endif