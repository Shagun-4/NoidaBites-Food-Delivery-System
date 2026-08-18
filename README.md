# NoidaBites — Mini Food Delivery System

A console based food delivery app (Mini Swiggy/Zomato) built in C++ for a DSA major project.
Delivery zones are modelled on Noida sectors, with a small road-distance graph used for
route finding and delivery pricing.

## Data structures used

| Feature                          | Data structure                          | Where |
|-----------------------------------|------------------------------------------|-------|
| Restaurant list + per-restaurant menu | Singly linked list                   | `Restaurant.h/.cpp` |
| Kitchen order processing          | Queue (FIFO, linked-list based)         | `OrderQueue.h/.cpp` |
| Completed order history           | Stack (LIFO, linked-list based)         | `OrderHistoryStack.h/.cpp` |
| Delivery route / distance         | Weighted undirected graph + Dijkstra    | `DeliveryGraph.h/.cpp` |
| Restaurant search                 | Linear substring search **and** binary search on a sorted snapshot | `Restaurant.cpp` |

Order records are also persisted to plain text files in `data/` so restaurants and order
history survive between runs (`FileManager.h/.cpp`).

## Project layout

```
foodie/
├── include/        header files (.h)
├── src/             implementation files (.cpp)
├── data/            restaurants.txt, order_history.txt (created on first run)
├── Makefile
└── README.md
```

## Building and running

```bash
make          # builds the "noidabites" binary
./noidabites  # or: make run
```

Requires a C++17 compiler (g++ 9+ or equivalent).

`make clean` removes build artefacts and resets `data/` back to empty (so the app reseeds
sample restaurants on next run).

## How it works, briefly

- On first run, five sample restaurants (spread across five Noida sectors) are loaded with
  full menus and saved to `data/restaurants.txt`.
- The delivery zone graph has 10 Noida locations connected by roads with approximate distances used as graph edge weights.
  Dijkstra's algorithm (implemented directly, not via a library) finds the shortest route
  between a restaurant's zone and the customer's zone.
- Placing an order builds up a bill from item subtotal + 5% GST + a delivery charge computed
  from the shortest-path distance (₹15 base fee + ₹8/km), and pushes the order onto the
  kitchen queue.
- "Process next kitchen order" dequeues the oldest pending order, walks it through
  Placed → Preparing → Out for Delivery → Delivered, and pushes the completed order onto the
  history stack. The complete history is saved to `data/order_history.txt` when the program exits.
- Searching supports both a linear substring scan over the linked list and a binary search
  for an exact name match over a freshly sorted snapshot — two different search techniques
  as called out in the project's DSA requirements.

## Possible extensions

- Multiple items per restaurant with categories (starters/mains/desserts)
- Order cancellation (remove from queue by ID)
- A second graph algorithm (e.g. A* for larger zone maps) for comparison
- Simple login/session handling for multiple customers
