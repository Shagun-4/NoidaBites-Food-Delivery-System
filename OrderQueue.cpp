#include "OrderQueue.h"
#include <iostream>

OrderQueue::OrderQueue() : frontPtr(nullptr), rearPtr(nullptr), count(0) {}

OrderQueue::~OrderQueue() {
    QueueNode* cur = frontPtr;
    while (cur) {
        QueueNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
}

void OrderQueue::enqueue(const Order& o) {
    QueueNode* node = new QueueNode(o);
    if (!rearPtr) { frontPtr = rearPtr = node; }
    else { rearPtr->next = node; rearPtr = node; }
    count++;
}

bool OrderQueue::dequeue(Order& out) {
    if (!frontPtr) return false;
    QueueNode* old = frontPtr;
    out = old->data;
    frontPtr = frontPtr->next;
    if (!frontPtr) rearPtr = nullptr;
    delete old;
    count--;
    return true;
}

bool OrderQueue::peekFront(Order& out) const {
    if (!frontPtr) return false;
    out = frontPtr->data;
    return true;
}

bool OrderQueue::isEmpty() const { return frontPtr == nullptr; }
int OrderQueue::size() const { return count; }

void OrderQueue::display() const {
    if (!frontPtr) { std::cout << "  Order queue is empty.\n"; return; }
    std::cout << "  Pending orders (front -> rear):\n";
    QueueNode* cur = frontPtr;
    while (cur) {
        cur->data.printSummary();
        cur = cur->next;
    }
}
