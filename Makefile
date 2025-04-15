# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

# Directories
SRC := src
OBJ := obj
BIN := bin

### TARGETS
## TEST DATABASE:
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
test_db: $(TEST_DB)

# Rule for building the executable
$(TEST_DB): $(TEST_DB_OBJS)
	@mkdir -p $(BIN)
	$(CXX) $(TEST_DB_OBJS) -o $(TEST_DB)

## SERVER:
SERVER = $(BIN)/server
SERVER_SRCS = \
	$(SRC)/connection.cc \
	$(SRC)/server.cc \
	$(SRC)/newsapp.cc \
	$(SRC)/messageHandler.cc \
	$(SRC)/article.cc \
	$(SRC)/newsgroup.cc \
	$(SRC)/inMemory.cc \
	$(SRC)/servermain.cc
SERVER_OBJS = $(patsubst $(SRC)/%.cc, $(OBJ)/%.o, $(SERVER_SRCS))

server: $(SERVER)

$(SERVER): $(SERVER_OBJS)
	@mkdir -p $(BIN)
	$(CXX) $(SERVER_OBJS) -o $(SERVER)

# Rule for creating object files
$(OBJ)/%.o: $(SRC)/%.cc
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -rf $(OBJ) $(BIN)

# Phony targets (clean, all)
.PHONY: all clean test_db