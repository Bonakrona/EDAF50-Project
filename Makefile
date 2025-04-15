# Compiler settings
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

# Directories
SRC := src
OBJ := obj
BIN := bin

# Output Executable Name
TEST_DB = $(BIN)/TestDatabase

# Source and Object files
TEST_DB_SRCS := \
	$(SRC)/article.cc \
	$(SRC)/newsgroup.cc \
	$(SRC)/inMemory.cc \
	$(SRC)/TestDatabase.cc

TEST_DB_OBJS := $(patsubst $(SRC)/%.cc, $(OBJ)/%.o, $(TEST_DB_SRCS))

# Default target to build the executable
# all: 
test_db: $(TEST_DB)

# Rule for building the executable
$(TEST_DB): $(TEST_DB_OBJS)
	@mkdir -p $(BIN)
	$(CXX) $(TEST_DB_OBJS) -o $(TEST_DB)

# Rule for creating object files
$(OBJ)/%.o: $(SRC)/%.cc
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -rf $(OBJ) $(BIN)

# Phony targets (clean, all)
.PHONY: all clean test_db