#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>


struct OrderItem {
    std::string itemName;
    double price;
    int quantity;
};

enum class OrderStatus { PLACED, PREPARING, OUT_FOR_DELIVERY, DELIVERED };

std::string statusToString(OrderStatus s);

class Order {
public:
    int orderId;
    std::string customerName;
    std::string restaurantName;
    std::string restaurantZone;
    std::string customerZone;
    std::vector<OrderItem> items;
    double subtotal;
    double gst;
    double deliveryCharge;
    double deliveryDistanceKm;
    double totalBill;
    OrderStatus status;
    std::string placedAt; 

    Order();
    Order(int id, const std::string& custName, const std::string& restName,
          const std::string& restZone, const std::string& custZone);

    void addItem(const std::string& name, double price, int qty);
    void calculateBill(double distanceKm, double deliveryChargePerKm, double baseDeliveryFee, double gstRate);
    void printReceipt() const;
    void printSummary() const; 

    std::string serialize() const;
    static Order deserialize(const std::string& line);
};

#endif
