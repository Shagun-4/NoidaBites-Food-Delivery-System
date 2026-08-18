# NoidaBites — Mini Food Delivery System

A console based food delivery system built in C++ as a DSA major project. The project simulates the basic working of an online food delivery application, including restaurant management, menu handling, order processing, order history, billing, restaurant searching, and delivery route calculation.

The delivery network is based on selected Noida locations, making the project more specific than a generic food delivery system.

## Features

- Browse all available restaurants
- Search restaurants by name
- Place food orders
- Calculate the total bill
- Process orders in FIFO sequence
- View pending orders
- View completed order history
- Find the shortest delivery route between zones
- Add restaurants through the admin option
- Add menu items through the admin option
- Remove restaurants through the admin option
- Save restaurant and order-history data between runs

## Data Structures and Algorithms Used

| Requirement | Data Structure / Algorithm | Implementation |
|---|---|---|
| Restaurant management | Singly Linked List | `Restaurant.h`, `Restaurant.cpp` |
| Menu management | Singly Linked List | `Restaurant.h`, `Restaurant.cpp` |
| Restaurant searching | Linear Search | `Restaurant.cpp` |
| Exact restaurant searching | Binary Search on a sorted snapshot | `Restaurant.cpp` |
| Order processing | Queue (FIFO) using linked nodes | `OrderQueue.h`, `OrderQueue.cpp` |
| Order history | Stack (LIFO) using linked nodes | `OrderHistoryStack.h`, `OrderHistoryStack.cpp` |
| Delivery route | Weighted Undirected Graph | `DeliveryGraph.h`, `DeliveryGraph.cpp` |
| Shortest route | Dijkstra's Algorithm | `DeliveryGraph.cpp` |
| Data persistence | Text-file handling | `FileManager.h`, `FileManager.cpp` |

## Project Structure

    NoidaBites-Food-Delivery-System/
    │
    ├── DeliveryGraph.cpp
    ├── DeliveryGraph.h
    ├── FileManager.cpp
    ├── FileManager.h
    ├── Makefile
    ├── Order.cpp
    ├── Order.h
    ├── OrderHistoryStack.cpp
    ├── OrderHistoryStack.h
    ├── OrderQueue.cpp
    ├── OrderQueue.h
    ├── README.md
    ├── Restaurant.cpp
    ├── Restaurant.h
    └── main.cpp

### File Overview

- **`main.cpp`** — Main program, menu system, user interaction, ordering flow, and application control.
- **`Restaurant.cpp / Restaurant.h`** — Restaurant and menu linked-list implementation and restaurant searching.
- **`Order.cpp / Order.h`** — Order details, ordered items, bill calculation, and order status.
- **`OrderQueue.cpp / OrderQueue.h`** — FIFO queue used for processing pending kitchen orders.
- **`OrderHistoryStack.cpp / OrderHistoryStack.h`** — LIFO stack used for completed order history.
- **`DeliveryGraph.cpp / DeliveryGraph.h`** — Noida delivery-zone graph and Dijkstra shortest-path implementation.
- **`FileManager.cpp / FileManager.h`** — Saving and loading restaurant and order-history information.
- **`Makefile`** — Commands for compiling, running, and cleaning the project.
- **`README.md`** — Project documentation.

## How the System Works

### 1. Restaurants and Menus

Restaurants are stored using a **singly linked list**. Each restaurant also maintains its menu using a linked list of menu items.

The project starts with sample restaurants based on Noida locations. Additional restaurants and menu items can be added using the admin options.

### 2. Restaurant Search

Two search techniques are included:

- **Linear Search** for partial or substring matches.
- **Binary Search** for exact restaurant-name matching using a sorted snapshot of restaurant pointers.

This demonstrates two different searching approaches as required by the project.

### 3. Ordering

A customer selects a restaurant, chooses food items and quantities, and places an order.

The bill consists of:

- Food item subtotal
- 5% GST
- Delivery charge based on the calculated delivery distance

### 4. Queue-Based Order Processing

New orders are inserted into a **FIFO queue**.

The oldest pending order is processed first, representing how a kitchen/order-processing system can handle orders in sequence.

Order status moves through:

    Placed → Preparing → Out for Delivery → Delivered

### 5. Stack-Based Order History

After an order is completed, it is pushed onto a **stack**.

Therefore, the most recently completed order appears first when viewing order history, demonstrating the **LIFO** principle.

### 6. Delivery Route

The delivery network is represented as a **weighted undirected graph** containing selected Noida locations.

Edge weights represent approximate distances between connected locations.

**Dijkstra's algorithm** is used to find the shortest route between the restaurant's zone and the customer's zone.

### 7. File Persistence

Restaurant information and completed order history are stored in text files so that important data can be loaded again when the application is restarted.

The program creates the required data files when needed.

## Example Noida Delivery Zones

The graph includes locations such as:

- Sector 18
- Sector 15
- Botanical Garden
- Film City
- Sector 62
- Sector 76
- Sector 50
- Sector 137
- Sector 128
- Noida Extension

These locations are connected using approximate graph distances for the route-finding component.

## Building and Running

### Requirements

- C++17 compatible compiler
- `g++`
- `make`

### Compile

From the project directory:

    make

### Run

    ./noidabites

or:

    make run

### Clean Build Files

    make clean

## Main Menu

The application provides options for:

    1. Browse all restaurants
    2. Search restaurants
    3. Place an order
    4. Process next kitchen order
    5. View pending orders
    6. View order history
    7. Check delivery route between zones
    8. [Admin] Add restaurant
    9. [Admin] Add menu item
    10. [Admin] Remove restaurant
    11. Save & Exit

## Learning Outcomes

This project demonstrates how common data structures can be applied to a real-world problem:

- **Linked Lists** for dynamic restaurant and menu management
- **Queues** for first-in-first-out order processing
- **Stacks** for last-in-first-out order history
- **Graphs** for modelling a delivery network
- **Dijkstra's Algorithm** for shortest-path calculation
- **Searching Techniques** for finding restaurants
- **File Handling** for basic data persistence
- **Modular C++ Programming** using header and implementation files

## Project Goal

The main goal of **NoidaBites** is to demonstrate the practical use of multiple DSA concepts in one application rather than building a full commercial food-delivery platform.
