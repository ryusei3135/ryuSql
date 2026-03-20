CXX = g++
CXXFLAGS = -O2 -std=c++23 -Iinclude

BUILD = build
SRC = $(shell find src -name "*.cpp")
OBJ = $(patsubst src/%.cpp,$(BUILD)/%.o,$(SRC))

TARGET = db

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD) $(TARGET)