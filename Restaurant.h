#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>
#include <vector>

// ---------- Menu: singly linked list of dishes belonging to one restaurant ----------
struct MenuItemNode {
    std::string name;
    double price;
    bool isVeg;
    int prepTimeMinutes;
    MenuItemNode* next;
    MenuItemNode(const std::string& n, double p, bool veg, int t)
        : name(n), price(p), isVeg(veg), prepTimeMinutes(t), next(nullptr) {}
};

class MenuList {
private:
    MenuItemNode* head;
    int itemCount;
public:
    MenuList();
    ~MenuList();
    // Rule of three: a restaurant list copies restaurants, each holding a MenuList
    MenuList(const MenuList& other);
    MenuList& operator=(const MenuList& other);

    void addItem(const std::string& name, double price, bool isVeg, int prepTime);
    bool removeItem(const std::string& name);
    MenuItemNode* find(const std::string& name) const;
    MenuItemNode* getHead() const { return head; }
    int size() const { return itemCount; }
    void display() const;
};

// ---------- Restaurant: node in a singly linked list of all restaurants ----------
struct RestaurantNode {
    int id;
    std::string name;
    std::string zone;      // delivery zone / Noida sector, must match a DeliveryGraph zone
    std::string cuisine;
    float rating;
    MenuList menu;
    RestaurantNode* next;
    RestaurantNode(int _id, const std::string& n, const std::string& z,
                   const std::string& c, float r)
        : id(_id), name(n), zone(z), cuisine(c), rating(r), next(nullptr) {}
};

class RestaurantList {
private:
    RestaurantNode* head;
    int nextId;
public:
    RestaurantList();
    ~RestaurantList();

    RestaurantNode* addRestaurant(const std::string& name, const std::string& zone,
                                   const std::string& cuisine, float rating, int forcedId = -1);
    bool removeRestaurant(const std::string& name);
    RestaurantNode* getHead() const { return head; }
    int count() const;

    // Searching (two techniques, as required by the "Searching" component):
    // 1) Linear/substring search directly over the linked list (case-insensitive).
    std::vector<RestaurantNode*> searchByNameContains(const std::string& query) const;
    // 2) Binary search for an exact name match, over a freshly sorted snapshot vector.
    RestaurantNode* searchByNameExactBinary(const std::string& exactName) const;

    void displayAll() const;
};

#endif
