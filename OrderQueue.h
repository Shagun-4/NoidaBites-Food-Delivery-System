#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include "Order.h"

// Linked-list based FIFO queue: orders are processed kitchen-side in the
// order they were placed (first placed, first prepared).
struct QueueNode {
    Order data;
    QueueNode* next;
    QueueNode(const Order& o) : data(o), next(nullptr) {}
};

class OrderQueue {
private:
    QueueNode* frontPtr;
    QueueNode* rearPtr;
    int count;
public:
    OrderQueue();
    ~OrderQueue();

    void enqueue(const Order& o);
    bool dequeue(Order& out);      // removes and returns the front order
    bool peekFront(Order& out) const;
    bool isEmpty() const;
    int size() const;
    void display() const;          // shows all pending orders, front to rear
};

#endif
