#include "Order.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

std::string statusToString(OrderStatus s) {
    switch (s) {
        case OrderStatus::PLACED: return "Placed";
        case OrderStatus::PREPARING: return "Preparing";
        case OrderStatus::OUT_FOR_DELIVERY: return "Out for Delivery";
        case OrderStatus::DELIVERED: return "Delivered";
    }
    return "Unknown";
}

static std::string currentTimestamp() {
    std::time_t t = std::time(nullptr);
    std::tm* tmPtr = std::localtime(&t);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M", tmPtr);
    return std::string(buf);
}

Order::Order()
    : orderId(0), subtotal(0), gst(0), deliveryCharge(0),
      deliveryDistanceKm(0), totalBill(0), status(OrderStatus::PLACED) {}

Order::Order(int id, const std::string& custName, const std::string& restName,
             const std::string& restZone, const std::string& custZone)
    : orderId(id), customerName(custName), restaurantName(restName),
      restaurantZone(restZone), customerZone(custZone),
      subtotal(0), gst(0), deliveryCharge(0), deliveryDistanceKm(0),
      totalBill(0), status(OrderStatus::PLACED) {
    placedAt = currentTimestamp();
}

void Order::addItem(const std::string& name, double price, int qty) {
    items.push_back({name, price, qty});
    subtotal += price * qty;
}

void Order::calculateBill(double distanceKm, double deliveryChargePerKm, double baseDeliveryFee, double gstRate) {
    deliveryDistanceKm = distanceKm;
    deliveryCharge = baseDeliveryFee + distanceKm * deliveryChargePerKm;
    gst = subtotal * gstRate;
    totalBill = subtotal + gst + deliveryCharge;
}

void Order::printReceipt() const {
    std::cout << "\n----------------------------------------------\n";
    std::cout << "  Order #" << orderId << "   (" << placedAt << ")\n";
    std::cout << "  Customer : " << customerName << "  [" << customerZone << "]\n";
    std::cout << "  Restaurant: " << restaurantName << "  [" << restaurantZone << "]\n";
    std::cout << "----------------------------------------------\n";
    std::cout << std::left << std::setw(24) << "Item" << std::setw(6) << "Qty" << "Price\n";
    for (const auto& it : items) {
        std::cout << std::left << std::setw(24) << it.itemName
                   << std::setw(6) << it.quantity
                   << std::fixed << std::setprecision(2) << (it.price * it.quantity) << "\n";
    }
    std::cout << "----------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Subtotal        : Rs. " << subtotal << "\n";
    std::cout << "  GST (5%)        : Rs. " << gst << "\n";
    std::cout << "  Delivery (" << deliveryDistanceKm << " km) : Rs. " << deliveryCharge << "\n";
    std::cout << "  TOTAL           : Rs. " << totalBill << "\n";
    std::cout << "  Status          : " << statusToString(status) << "\n";
    std::cout << "----------------------------------------------\n";
}

void Order::printSummary() const {
    std::cout << "  #" << orderId << " | " << customerName
              << " -> " << restaurantName
              << " | Rs." << std::fixed << std::setprecision(2) << totalBill
              << " | " << statusToString(status)
              << " | " << placedAt << "\n";
}

std::string Order::serialize() const {
    std::ostringstream oss;
    oss << orderId << "|" << customerName << "|" << restaurantName << "|"
        << restaurantZone << "|" << customerZone << "|" << subtotal << "|"
        << gst << "|" << deliveryCharge << "|" << deliveryDistanceKm << "|"
        << totalBill << "|" << static_cast<int>(status) << "|" << placedAt << "|";
    for (size_t i = 0; i < items.size(); ++i) {
        oss << items[i].itemName << "," << items[i].price << "," << items[i].quantity;
        if (i + 1 < items.size()) oss << ";";
    }
    return oss.str();
}

static std::vector<std::string> splitPipe(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, '|')) parts.push_back(field);
    return parts;
}

Order Order::deserialize(const std::string& line) {
    Order o;
    auto parts = splitPipe(line);
    if (parts.size() < 12) return o; // malformed line, return empty order
    o.orderId = std::stoi(parts[0]);
    o.customerName = parts[1];
    o.restaurantName = parts[2];
    o.restaurantZone = parts[3];
    o.customerZone = parts[4];
    o.subtotal = std::stod(parts[5]);
    o.gst = std::stod(parts[6]);
    o.deliveryCharge = std::stod(parts[7]);
    o.deliveryDistanceKm = std::stod(parts[8]);
    o.totalBill = std::stod(parts[9]);
    o.status = static_cast<OrderStatus>(std::stoi(parts[10]));
    o.placedAt = parts[11];
    if (parts.size() >= 13 && !parts[12].empty()) {
        std::stringstream itemStream(parts[12]);
        std::string itemToken;
        while (std::getline(itemStream, itemToken, ';')) {
            std::stringstream itemFields(itemToken);
            std::string name, priceStr, qtyStr;
            std::getline(itemFields, name, ',');
            std::getline(itemFields, priceStr, ',');
            std::getline(itemFields, qtyStr, ',');
            if (!name.empty())
                o.items.push_back({name, std::stod(priceStr), std::stoi(qtyStr)});
        }
    }
    return o;
}
