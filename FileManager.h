#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "Restaurant.h"
#include "OrderHistoryStack.h"
#include <string>

// Handles saving/loading restaurant + menu data and order history to
// plain text files under data/, so the app remembers state across runs.
namespace FileManager {
    void saveRestaurants(const RestaurantList& list, const std::string& path);
    void loadRestaurants(RestaurantList& list, const std::string& path);

    void saveOrderHistory(const OrderHistoryStack& stack, const std::string& path);
    void loadOrderHistory(OrderHistoryStack& stack, const std::string& path);

    void appendOrderLine(const Order& o, const std::string& path);
    bool fileExists(const std::string& path);
}

#endif
