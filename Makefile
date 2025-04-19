CXX = g++
CXXFLAGS = -IModels -IDeliverySystem -Wall
SRC = main.cpp $(wildcard DeliverySystem/*.cpp) $(wildcard Models/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = app

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# $@ → the target name (here, app)
# $^ → all the dependencies (here, the .cpp files)

clean:
	rm -f $(TARGET)
