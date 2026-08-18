#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

bool FileManager::fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

// Restaurant file format (pipe-delimited, one entity per line):
//   R|id|name|zone|cuisine|rating
//   M|restaurantId|name|price|isVeg(0/1)|prepTimeMinutes
// Menu lines always immediately relate to the most recently written R line.
void FileManager::saveRestaurants(const RestaurantList& list, const std::string& path) {
    std::ofstream out(path);
    if (!out) { std::cerr << "  Could not open " << path << " for writing.\n"; return; }
    RestaurantNode* cur = list.getHead();
    while (cur) {
        out << "R|" << cur->id << "|" << cur->name << "|" << cur->zone << "|"
            << cur->cuisine << "|" << cur->rating << "\n";
        MenuItemNode* m = cur->menu.getHead();
        while (m) {
            out << "M|" << cur->id << "|" << m->name << "|" << m->price << "|"
                << (m->isVeg ? 1 : 0) << "|" << m->prepTimeMinutes << "\n";
            m = m->next;
        }
        cur = cur->next;
    }
}

void FileManager::loadRestaurants(RestaurantList& list, const std::string& path) {
    std::ifstream in(path);
    if (!in) return; // nothing to load yet, that's fine on first run

    std::string line;
    RestaurantNode* lastAdded = nullptr;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string tag;
        std::getline(ss, tag, '|');

        if (tag == "R") {
            std::string idStr, name, zone, cuisine, ratingStr;
            std::getline(ss, idStr, '|');
            std::getline(ss, name, '|');
            std::getline(ss, zone, '|');
            std::getline(ss, cuisine, '|');
            std::getline(ss, ratingStr, '|');
            lastAdded = list.addRestaurant(name, zone, cuisine, std::stof(ratingStr), std::stoi(idStr));
        } else if (tag == "M" && lastAdded) {
            std::string ridStr, name, priceStr, vegStr, prepStr;
            std::getline(ss, ridStr, '|');
            std::getline(ss, name, '|');
            std::getline(ss, priceStr, '|');
            std::getline(ss, vegStr, '|');
            std::getline(ss, prepStr, '|');
            if (std::stoi(ridStr) == lastAdded->id) {
                lastAdded->menu.addItem(name, std::stod(priceStr), vegStr == "1", std::stoi(prepStr));
            }
        }
    }
}

void FileManager::saveOrderHistory(const OrderHistoryStack& stack, const std::string& path) {
    std::ofstream out(path);
    if (!out) { std::cerr << "  Could not open " << path << " for writing.\n"; return; }
    // toVector() returns top-of-stack first; write in that order so the
    // most recent order is the first line for quick inspection.
    for (const auto& o : stack.toVector()) out << o.serialize() << "\n";
}

void FileManager::loadOrderHistory(OrderHistoryStack& stack, const std::string& path) {
    std::ifstream in(path);
    if (!in) return;
    std::vector<Order> loaded;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        loaded.push_back(Order::deserialize(line));
    }
    // File stores most-recent-first; push in reverse so stack order
    // (top = most recent) is preserved after reload.
    for (auto it = loaded.rbegin(); it != loaded.rend(); ++it) stack.push(*it);
}

void FileManager::appendOrderLine(const Order& o, const std::string& path) {
    std::ofstream out(path, std::ios::app);
    if (!out) return;
    out << o.serialize() << "\n";
}
