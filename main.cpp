// ============================================================
//  NoidaBites — Mini Food Delivery System
//  Major Project (DSA in C++)
//
//  Data structures used:
//    Linked List  -> Restaurant.h / Restaurant.cpp  (restaurants + per-restaurant menu)
//    Queue        -> OrderQueue.h / OrderQueue.cpp   (kitchen order processing, FIFO)
//    Stack        -> OrderHistoryStack.h / .cpp      (completed order history, LIFO)
//    Graph        -> DeliveryGraph.h / .cpp          (Noida zones + Dijkstra route/distance)
//    Searching    -> Restaurant.cpp                  (linear substring + binary exact-match)
// ============================================================

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>

#include "Restaurant.h"
#include "OrderQueue.h"
#include "OrderHistoryStack.h"
#include "DeliveryGraph.h"
#include "FileManager.h"
#include "Order.h"

const std::string RESTAURANT_FILE = "data/restaurants.txt";
const std::string HISTORY_FILE = "data/order_history.txt";

const double GST_RATE = 0.05;              // 5% GST on food subtotal
const double DELIVERY_RATE_PER_KM = 8.0;   // Rs. per km
const double BASE_DELIVERY_FEE = 15.0;     // Rs. flat fee

// ---------------------------------------------------------------
// input helper functions
// ---------------------------------------------------------------
int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return value; }
        std::cout << "  Please enter a valid number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return value; }
        std::cout << "  Please enter a valid number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

char readYesNo(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt + " (y/n): ");
        if (!s.empty() && (s[0] == 'y' || s[0] == 'Y')) return 'y';
        if (!s.empty() && (s[0] == 'n' || s[0] == 'N')) return 'n';
        std::cout << "  Please answer y or n.\n";
    }
}

void pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

// ---------------------------------------------------------------
// setup: Noida delivery zone graph
// ---------------------------------------------------------------
void setupNoidaGraph(DeliveryGraph& graph) {
    const char* zones[] = {
        "Sector 18", "Sector 15", "Botanical Garden", "Film City",
        "Sector 62", "Sector 76", "Sector 50", "Sector 137",
        "Sector 128", "Noida Extension"
    };
    for (const char* z : zones) graph.addZone(z);

    graph.addRoad("Sector 18", "Sector 15", 3.0);
    graph.addRoad("Sector 18", "Botanical Garden", 2.5);
    graph.addRoad("Sector 18", "Film City", 2.0);
    graph.addRoad("Sector 15", "Botanical Garden", 2.0);
    graph.addRoad("Sector 15", "Film City", 3.5);
    graph.addRoad("Botanical Garden", "Sector 62", 4.0);
    graph.addRoad("Sector 62", "Sector 76", 5.0);
    graph.addRoad("Sector 76", "Sector 50", 3.0);
    graph.addRoad("Sector 76", "Sector 137", 7.0);
    graph.addRoad("Sector 50", "Sector 137", 6.0);
    graph.addRoad("Sector 137", "Sector 128", 2.0);
    graph.addRoad("Sector 128", "Noida Extension", 4.0);
    graph.addRoad("Noida Extension", "Sector 137", 5.0);
}

// Add sample restaurants for first run
void seedRestaurants(RestaurantList& list) {
    RestaurantNode* r1 = list.addRestaurant("Punjabi Tadka", "Sector 18", "North Indian", 4.3f);
    r1->menu.addItem("Butter Chicken", 320, false, 20);
    r1->menu.addItem("Dal Makhani", 220, true, 15);
    r1->menu.addItem("Tandoori Roti", 40, true, 5);
    r1->menu.addItem("Paneer Tikka", 260, true, 18);
    r1->menu.addItem("Chicken Biryani", 300, false, 25);

    RestaurantNode* r2 = list.addRestaurant("Noida Wok House", "Sector 62", "Chinese", 4.1f);
    r2->menu.addItem("Veg Hakka Noodles", 180, true, 12);
    r2->menu.addItem("Chicken Manchurian", 260, false, 18);
    r2->menu.addItem("Spring Rolls", 150, true, 10);
    r2->menu.addItem("Chilli Paneer", 220, true, 15);

    RestaurantNode* r3 = list.addRestaurant("The Pizza Yard", "Film City", "Italian", 4.5f);
    r3->menu.addItem("Margherita Pizza", 250, true, 15);
    r3->menu.addItem("Farmhouse Pizza", 320, true, 18);
    r3->menu.addItem("Pepperoni Pizza", 380, false, 20);
    r3->menu.addItem("Garlic Bread", 120, true, 10);

    RestaurantNode* r4 = list.addRestaurant("South Spice Express", "Sector 137", "South Indian", 4.4f);
    r4->menu.addItem("Masala Dosa", 130, true, 12);
    r4->menu.addItem("Idli Sambar", 90, true, 8);
    r4->menu.addItem("Filter Coffee", 40, true, 3);
    r4->menu.addItem("Chicken Chettinad", 280, false, 22);

    RestaurantNode* r5 = list.addRestaurant("Burger Junction", "Sector 50", "Fast Food", 4.0f);
    r5->menu.addItem("Classic Veg Burger", 140, true, 8);
    r5->menu.addItem("Crispy Chicken Burger", 190, false, 10);
    r5->menu.addItem("Fries", 90, true, 6);
    r5->menu.addItem("Cold Coffee", 110, true, 4);
}

// ---------------------------------------------------------------
// feature: restaurant search
// ---------------------------------------------------------------
void handleSearch(const RestaurantList& restaurants) {
    std::cout << "\n  1. Search by partial name (linear scan)\n";
    std::cout << "  2. Search by exact name (binary search on sorted snapshot)\n";
    int choice = readInt("  Choose: ");
    if (choice == 1) {
        std::string q = readLine("  Enter search text: ");
        auto results = restaurants.searchByNameContains(q);
        if (results.empty()) { std::cout << "  No matches found.\n"; return; }
        std::cout << "  Found " << results.size() << " match(es):\n";
        for (auto* r : results)
            std::cout << "    [" << r->id << "] " << r->name << " - " << r->cuisine
                       << " - Zone: " << r->zone << " - Rating: " << r->rating << "\n";
    } else if (choice == 2) {
        std::string q = readLine("  Enter exact restaurant name: ");
        RestaurantNode* r = restaurants.searchByNameExactBinary(q);
        if (!r) { std::cout << "  No exact match found.\n"; return; }
        std::cout << "  Found: [" << r->id << "] " << r->name << " - " << r->cuisine
                   << " - Zone: " << r->zone << " - Rating: " << r->rating << "\n";
    } else {
        std::cout << "  Invalid choice.\n";
    }
}

// ---------------------------------------------------------------
// feature: place an order
// ---------------------------------------------------------------
void handlePlaceOrder(RestaurantList& restaurants, OrderQueue& queue,
                       DeliveryGraph& graph, int& orderIdCounter) {
    if (restaurants.count() == 0) { std::cout << "  No restaurants available yet.\n"; return; }

    restaurants.displayAll();
    std::string rName = readLine("\n  Enter restaurant name exactly as shown: ");
    RestaurantNode* rest = restaurants.searchByNameExactBinary(rName);
    if (!rest) { std::cout << "  Restaurant not found.\n"; return; }

    std::cout << "\n  Menu for " << rest->name << ":\n";
    rest->menu.display();

    std::string custName = readLine("\n  Your name: ");
    std::string custZone;
    while (true) {
        custZone = readLine("  Your delivery zone (e.g. Sector 62): ");
        if (graph.hasZone(custZone)) break;
        std::cout << "  Unknown zone. Available zones:\n";
        for (const auto& z : graph.zones()) std::cout << "    - " << z << "\n";
    }

    Order order(orderIdCounter, custName, rest->name, rest->zone, custZone);

    char more = 'y';
    while (more == 'y') {
        std::string itemName = readLine("  Item name to add: ");
        MenuItemNode* item = rest->menu.find(itemName);
        if (!item) { std::cout << "  Item not on menu. Try again.\n"; more = readYesNo("  Add another item?"); continue; }
        int qty = readInt("  Quantity: ");
        if (qty <= 0) { std::cout << "  Quantity must be positive.\n"; more = readYesNo("  Add another item?"); continue; }
        order.addItem(item->name, item->price, qty);
        more = readYesNo("  Add another item?");
    }

    if (order.items.empty()) { std::cout << "  No items added, order cancelled.\n"; return; }

    double distanceKm;
    std::vector<std::string> path;
    if (!graph.shortestPath(rest->zone, custZone, distanceKm, path)) {
        std::cout << "  No delivery route found between zones! Order cancelled.\n";
        return;
    }
    order.calculateBill(distanceKm, DELIVERY_RATE_PER_KM, BASE_DELIVERY_FEE, GST_RATE);
    order.printReceipt();

    std::cout << "  Delivery route: ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i];
        if (i + 1 < path.size()) std::cout << " -> ";
    }
    std::cout << "  (" << distanceKm << " km)\n";

    queue.enqueue(order);
    std::cout << "\n  Order #" << order.orderId << " placed and sent to the kitchen queue!\n";
    orderIdCounter++;
}

// ---------------------------------------------------------------
// feature: process the next order in the kitchen queue
// ---------------------------------------------------------------
void handleProcessOrder(OrderQueue& queue, OrderHistoryStack& history) {
    Order o;
    if (!queue.dequeue(o)) { std::cout << "  No pending orders to process.\n"; return; }

    std::cout << "  Processing order #" << o.orderId << " for " << o.customerName << "...\n";
    o.status = OrderStatus::PREPARING;
    std::cout << "    -> " << statusToString(o.status) << "\n";
    o.status = OrderStatus::OUT_FOR_DELIVERY;
    std::cout << "    -> " << statusToString(o.status) << "\n";
    o.status = OrderStatus::DELIVERED;
    std::cout << "    -> " << statusToString(o.status) << "\n";

    history.push(o);
    std::cout << "  Order #" << o.orderId << " delivered and moved to order history.\n";
}

// ---------------------------------------------------------------
// feature: delivery route lookup between any two zones
// ---------------------------------------------------------------
void handleRouteQuery(DeliveryGraph& graph) {
    graph.printNetwork();
    std::string from = readLine("\n  From zone: ");
    std::string to = readLine("  To zone: ");
    double distanceKm;
    std::vector<std::string> path;
    if (!graph.shortestPath(from, to, distanceKm, path)) {
        std::cout << "  Route not found (check zone names).\n";
        return;
    }
    std::cout << "  Shortest route (" << distanceKm << " km): ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i];
        if (i + 1 < path.size()) std::cout << " -> ";
    }
    std::cout << "\n";
}

// ---------------------------------------------------------------
// admin actions
// ---------------------------------------------------------------
void handleAddRestaurant(RestaurantList& restaurants, DeliveryGraph& graph) {
    std::string name = readLine("  Restaurant name: ");
    std::string zone;
    while (true) {
        zone = readLine("  Zone (must be an existing Noida zone): ");
        if (graph.hasZone(zone)) break;
        std::cout << "  Unknown zone. Available zones:\n";
        for (const auto& z : graph.zones()) std::cout << "    - " << z << "\n";
    }
    std::string cuisine = readLine("  Cuisine type: ");
    double rating;
    while (true) {
        rating = readDouble("  Rating (0-5): ");
        if (rating >= 0.0 && rating <= 5.0) break;
        std::cout << "  Rating must be between 0 and 5.\n";
    }
    restaurants.addRestaurant(name, zone, cuisine, static_cast<float>(rating));
    std::cout << "  Restaurant added.\n";
}

void handleAddMenuItem(RestaurantList& restaurants) {
    restaurants.displayAll();
    std::string rName = readLine("\n  Restaurant name to add item to: ");
    RestaurantNode* rest = restaurants.searchByNameExactBinary(rName);
    if (!rest) { std::cout << "  Restaurant not found.\n"; return; }
    std::string itemName = readLine("  Item name: ");
    double price;
    while (true) {
        price = readDouble("  Price (Rs.): ");
        if (price > 0.0) break;
        std::cout << "  Price must be greater than 0.\n";
    }
    char vegAns = readYesNo("  Is it vegetarian?");
    int prepTime;
    while (true) {
        prepTime = readInt("  Approx prep time (minutes): ");
        if (prepTime > 0) break;
        std::cout << "  Prep time must be greater than 0.\n";
    }
    rest->menu.addItem(itemName, price, vegAns == 'y', prepTime);
    std::cout << "  Menu item added to " << rest->name << ".\n";
}

void handleRemoveRestaurant(RestaurantList& restaurants) {
    restaurants.displayAll();
    std::string rName = readLine("\n  Restaurant name to remove: ");
    if (restaurants.removeRestaurant(rName)) std::cout << "  Removed.\n";
    else std::cout << "  Restaurant not found.\n";
}

void handleRemoveMenuItem(RestaurantList& restaurants) {
    restaurants.displayAll();
    std::string rName = readLine("\n  Restaurant name: ");
    RestaurantNode* rest = restaurants.searchByNameExactBinary(rName);
    if (!rest) {
        std::cout << "  Restaurant not found.\n";
        return;
    }

    std::cout << "\n  Menu for " << rest->name << ":\n";
    rest->menu.display();
    std::string itemName = readLine("  Menu item to remove: ");
    if (rest->menu.removeItem(itemName)) std::cout << "  Menu item removed.\n";
    else std::cout << "  Menu item not found.\n";
}

// ---------------------------------------------------------------
// menu display
// ---------------------------------------------------------------
void printMainMenu() {
    std::cout << "\n================= NoidaBites =================\n";
    std::cout << " 1. Browse all restaurants\n";
    std::cout << " 2. Search restaurants\n";
    std::cout << " 3. Place an order\n";
    std::cout << " 4. Process next kitchen order (queue)\n";
    std::cout << " 5. View pending orders (queue)\n";
    std::cout << " 6. View order history (stack)\n";
    std::cout << " 7. Check delivery route between zones\n";
    std::cout << " 8. [Admin] Add restaurant\n";
    std::cout << " 9. [Admin] Add menu item\n";
    std::cout << "10. [Admin] Remove restaurant\n";
    std::cout << "11. [Admin] Remove menu item\n";
    std::cout << "12. Save & Exit\n";
    std::cout << "================================================\n";
}

int main() {
    RestaurantList restaurants;
    OrderQueue orderQueue;
    OrderHistoryStack orderHistory;
    DeliveryGraph deliveryGraph;

    setupNoidaGraph(deliveryGraph);

    bool freshInstall = !FileManager::fileExists(RESTAURANT_FILE);
    if (freshInstall) {
        seedRestaurants(restaurants);
        FileManager::saveRestaurants(restaurants, RESTAURANT_FILE);
    } else {
        FileManager::loadRestaurants(restaurants, RESTAURANT_FILE);
    }
    FileManager::loadOrderHistory(orderHistory, HISTORY_FILE);

    // figure out the next order id from whatever history was loaded
    int orderIdCounter = 1001;
    for (const auto& o : orderHistory.toVector()) {
        if (o.orderId >= orderIdCounter) orderIdCounter = o.orderId + 1;
    }

    std::cout << "Welcome to NoidaBites — Mini Food Delivery System\n";
    if (freshInstall) std::cout << "(First run detected — sample restaurants have been loaded.)\n";

    bool running = true;
    while (running) {
        printMainMenu();
        int choice = readInt("Choose an option: ");
        std::cout << "\n";
        switch (choice) {
            case 1: restaurants.displayAll(); break;
            case 2: handleSearch(restaurants); break;
            case 3: handlePlaceOrder(restaurants, orderQueue, deliveryGraph, orderIdCounter); break;
            case 4: handleProcessOrder(orderQueue, orderHistory); break;
            case 5: orderQueue.display(); break;
            case 6: orderHistory.displayRecent(10); break;
            case 7: handleRouteQuery(deliveryGraph); break;
            case 8: handleAddRestaurant(restaurants, deliveryGraph); break;
            case 9: handleAddMenuItem(restaurants); break;
            case 10: handleRemoveRestaurant(restaurants); break;
            case 11: handleRemoveMenuItem(restaurants); break;
            case 12:
                FileManager::saveRestaurants(restaurants, RESTAURANT_FILE);
                FileManager::saveOrderHistory(orderHistory, HISTORY_FILE);
                std::cout << "Data saved. Goodbye!\n";
                running = false;
                break;
            default:
                std::cout << "  Invalid option, try again.\n";
        }
        if (running && choice != 12) pause();
    }
    return 0;
}
