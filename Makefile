CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
SRC = src/Order.cpp src/Restaurant.cpp src/OrderQueue.cpp src/OrderHistoryStack.cpp \
      src/DeliveryGraph.cpp src/FileManager.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = noidabites

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
	rm -f data/restaurants.txt data/order_history.txt

run: all
	./$(TARGET)

.PHONY: all clean run
