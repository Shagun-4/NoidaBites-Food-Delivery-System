#ifndef ORDER_HISTORY_STACK_H
#define ORDER_HISTORY_STACK_H

#include "Order.h"
#include <vector>

// Linked-list based LIFO stack: once an order is delivered it is pushed
// here, so the most recently completed order is always seen first --
// mirrors how "recent orders" screens work on real apps.
struct StackNode {
    Order data;
    StackNode* next;
    StackNode(const Order& o) : data(o), next(nullptr) {}
};

class OrderHistoryStack {
private:
    StackNode* topPtr;
    int count;
public:
    OrderHistoryStack();
    ~OrderHistoryStack();

    void push(const Order& o);
    bool pop(Order& out);
    bool peekTop(Order& out) const;
    bool isEmpty() const;
    int size() const;

    void displayRecent(int n) const;   // most recent n, without modifying the stack
    std::vector<Order> toVector() const; // top-to-bottom snapshot, used for file save
};

#endif
