#include "OrderHistoryStack.h"
#include <iostream>

OrderHistoryStack::OrderHistoryStack() : topPtr(nullptr), count(0) {}

OrderHistoryStack::~OrderHistoryStack() {
    StackNode* cur = topPtr;
    while (cur) {
        StackNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
}

void OrderHistoryStack::push(const Order& o) {
    StackNode* node = new StackNode(o);
    node->next = topPtr;
    topPtr = node;
    count++;
}

bool OrderHistoryStack::pop(Order& out) {
    if (!topPtr) return false;
    StackNode* old = topPtr;
    out = old->data;
    topPtr = topPtr->next;
    delete old;
    count--;
    return true;
}

bool OrderHistoryStack::peekTop(Order& out) const {
    if (!topPtr) return false;
    out = topPtr->data;
    return true;
}

bool OrderHistoryStack::isEmpty() const { return topPtr == nullptr; }
int OrderHistoryStack::size() const { return count; }

void OrderHistoryStack::displayRecent(int n) const {
    if (!topPtr) { std::cout << "  No completed orders yet.\n"; return; }
    std::cout << "  Most recent completed orders:\n";
    StackNode* cur = topPtr;
    int shown = 0;
    while (cur && shown < n) {
        cur->data.printSummary();
        cur = cur->next;
        shown++;
    }
}

std::vector<Order> OrderHistoryStack::toVector() const {
    std::vector<Order> result;
    StackNode* cur = topPtr;
    while (cur) { result.push_back(cur->data); cur = cur->next; }
    return result;
}
