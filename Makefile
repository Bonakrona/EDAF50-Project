# Compiler settings
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Directories
SRC_DIR = .
OBJ_DIR = obj
BIN_DIR = bin

# Output Executable Name
EXEC = $(BIN_DIR)/TestDatabase

# Source and Object files
SRC_FILES = $(SRC_DIR)/src/inMemory.cc $(SRC_DIR)/src/article.cc $(SRC_DIR)/src/newsgroup.cc $(SRC_DIR)/src/TestDatabase.cc
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/src/%.cc=$(OBJ_DIR)/%.o)

# Default target to build the executable
all: $(EXEC)

# Rule for building the executable
$(EXEC): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $(EXEC)

# Rule for creating object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/src/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets (clean, all)
.PHONY: all clean