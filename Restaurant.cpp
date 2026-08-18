#include "Restaurant.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

// ------------------------- MenuList -------------------------

MenuList::MenuList() : head(nullptr), itemCount(0) {}

MenuList::~MenuList() {
    MenuItemNode* cur = head;
    while (cur) {
        MenuItemNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
}

MenuList::MenuList(const MenuList& other) : head(nullptr), itemCount(0) {
    MenuItemNode* cur = other.head;
    while (cur) {
        addItem(cur->name, cur->price, cur->isVeg, cur->prepTimeMinutes);
        cur = cur->next;
    }
}

MenuList& MenuList::operator=(const MenuList& other) {
    if (this == &other) return *this;
    MenuItemNode* cur = head;
    while (cur) { MenuItemNode* nxt = cur->next; delete cur; cur = nxt; }
    head = nullptr; itemCount = 0;
    cur = other.head;
    while (cur) {
        addItem(cur->name, cur->price, cur->isVeg, cur->prepTimeMinutes);
        cur = cur->next;
    }
    return *this;
}

void MenuList::addItem(const std::string& name, double price, bool isVeg, int prepTime) {
    MenuItemNode* node = new MenuItemNode(name, price, isVeg, prepTime);
    if (!head) { head = node; }
    else {
        MenuItemNode* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
    itemCount++;
}

bool MenuList::removeItem(const std::string& name) {
    MenuItemNode* cur = head;
    MenuItemNode* prev = nullptr;
    while (cur) {
        if (cur->name == name) {
            if (prev) prev->next = cur->next;
            else head = cur->next;
            delete cur;
            itemCount--;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

MenuItemNode* MenuList::find(const std::string& name) const {
    MenuItemNode* cur = head;
    while (cur) {
        if (cur->name == name) return cur;
        cur = cur->next;
    }
    return nullptr;
}

void MenuList::display() const {
    if (!head) { std::cout << "    (menu is empty)\n"; return; }
    MenuItemNode* cur = head;
    int i = 1;
    while (cur) {
        std::cout << "    " << i++ << ". " << std::left << std::setw(22) << cur->name
                   << (cur->isVeg ? "[Veg] " : "[Non-Veg] ")
                   << "Rs." << std::fixed << std::setprecision(2) << cur->price
                   << "  (~" << cur->prepTimeMinutes << " min)\n";
        cur = cur->next;
    }
}

// ------------------------- RestaurantList -------------------------

RestaurantList::RestaurantList() : head(nullptr), nextId(1) {}

RestaurantList::~RestaurantList() {
    RestaurantNode* cur = head;
    while (cur) {
        RestaurantNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
}

RestaurantNode* RestaurantList::addRestaurant(const std::string& name, const std::string& zone,
                                               const std::string& cuisine, float rating, int forcedId) {
    int idToUse = (forcedId > 0) ? forcedId : nextId;
    RestaurantNode* node = new RestaurantNode(idToUse, name, zone, cuisine, rating);
    if (!head) { head = node; }
    else {
        RestaurantNode* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
    if (idToUse >= nextId) nextId = idToUse + 1;
    return node;
}

bool RestaurantList::removeRestaurant(const std::string& name) {
    RestaurantNode* cur = head;
    RestaurantNode* prev = nullptr;
    while (cur) {
        if (cur->name == name) {
            if (prev) prev->next = cur->next;
            else head = cur->next;
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

int RestaurantList::count() const {
    int c = 0;
    RestaurantNode* cur = head;
    while (cur) { c++; cur = cur->next; }
    return c;
}

static std::string toLowerStr(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), [](unsigned char c) { return std::tolower(c); });
    return r;
}

std::vector<RestaurantNode*> RestaurantList::searchByNameContains(const std::string& query) const {
    std::vector<RestaurantNode*> results;
    std::string lowerQuery = toLowerStr(query);
    RestaurantNode* cur = head;
    while (cur) {
        if (toLowerStr(cur->name).find(lowerQuery) != std::string::npos) {
            results.push_back(cur);
        }
        cur = cur->next;
    }
    return results;
}

RestaurantNode* RestaurantList::searchByNameExactBinary(const std::string& exactName) const {
    // Build a sorted snapshot (vector of pointers into the live linked list nodes)
    std::vector<RestaurantNode*> snapshot;
    RestaurantNode* cur = head;
    while (cur) { snapshot.push_back(cur); cur = cur->next; }
    std::sort(snapshot.begin(), snapshot.end(), [](RestaurantNode* a, RestaurantNode* b) {
        return toLowerStr(a->name) < toLowerStr(b->name);
    });

    std::string target = toLowerStr(exactName);
    int lo = 0, hi = static_cast<int>(snapshot.size()) - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        std::string midName = toLowerStr(snapshot[mid]->name);
        if (midName == target) return snapshot[mid];
        else if (midName < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return nullptr;
}

void RestaurantList::displayAll() const {
    if (!head) { std::cout << "  No restaurants registered yet.\n"; return; }
    RestaurantNode* cur = head;
    while (cur) {
        std::cout << "  [" << cur->id << "] " << cur->name
                   << "  (" << cur->cuisine << ")  Zone: " << cur->zone
                   << "  Rating: " << cur->rating << "/5"
                   << "  Menu items: " << cur->menu.size() << "\n";
        cur = cur->next;
    }
}
