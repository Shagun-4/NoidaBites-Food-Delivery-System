#ifndef ORDER_HISTORY_STACK_H
#define ORDER_HISTORY_STACK_H

#include "Order.h"
#include <vector>


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

    void displayRecent(int n) const;   
    std::vector<Order> toVector() const; 
};

#endif
