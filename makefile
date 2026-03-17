CXX = g++
CXXFLAGS = -O2 -std=c++23 -Iinclude

SRC = $(shell find src -name "*.cpp")
OBJ = $(SRC:.cpp=.o)

TARGET = db

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)