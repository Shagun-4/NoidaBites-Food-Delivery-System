#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include "Order.h"


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
    bool dequeue(Order& out);      
    bool peekFront(Order& out) const;
    bool isEmpty() const;
    int size() const;
    void display() const;         
};

#endif
