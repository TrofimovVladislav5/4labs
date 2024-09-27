CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./modules/ImageFileReader -I./modules/Parser -I./modules/Structures

SRC_DIR = .
OBJ_DIR = ./build

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/modules/*/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

EXECUTABLE = cw

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(EXECUTABLE) -lpng

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)