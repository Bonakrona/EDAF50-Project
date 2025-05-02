# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

# Directories
SRC := src
OBJ := obj
BIN := bin

CXXFLAGS_MEM := $(CXXFLAGS) -DUSE_INMEMORY
CXXFLAGS_DISC := $(CXXFLAGS) -DUSE_INDISC

### TARGETS
# Macro for building the executable
define build_executable
$1: $2
	@mkdir -p $(BIN)
	$(CXX) $2 -o $1
endef

## ALL:
all: server_mem server_disc client

## TEST DATABASE:
# Output Executable Name
TEST_DB = $(BIN)/TestDatabase

# Source and Object files
TEST_DB_SRCS := \
	$(SRC)/article.cc \
	$(SRC)/newsgroup.cc \
	$(SRC)/inMemory.cc \
	$(SRC)/inDisc.cc \
	$(SRC)/TestDatabase.cc

TEST_DB_OBJS := $(patsubst $(SRC)/%.cc, $(OBJ)/%.o, $(TEST_DB_SRCS))

test_db: $(TEST_DB)

$(eval $(call build_executable,$(TEST_DB),$(TEST_DB_OBJS)))

## SERVER:
SERVER_MEM = $(BIN)/server_mem
SERVER_DISC = $(BIN)/server_disc

SERVER_MEM_SRCS = \
	$(SRC)/connection.cc \
	$(SRC)/server.cc \
	$(SRC)/newsapp.cc \
	$(SRC)/messageHandler.cc \
	$(SRC)/article.cc \
	$(SRC)/newsgroup.cc \
	$(SRC)/inMemory.cc \
	$(SRC)/servermain.cc

SERVER_DISC_SRCS = \
	$(SRC)/connection.cc \
	$(SRC)/server.cc \
	$(SRC)/newsapp.cc \
	$(SRC)/messageHandler.cc \
	$(SRC)/article.cc \
	$(SRC)/newsgroup.cc \
	$(SRC)/inDisc.cc \
	$(SRC)/servermain.cc

SERVER_MEM_OBJS = $(patsubst $(SRC)/%.cc, $(OBJ)/%.o, $(SERVER_MEM_SRCS))
SERVER_DISC_OBJS = $(patsubst $(SRC)/%.cc, $(OBJ)/%.o, $(SERVER_DISC_SRCS))

server_mem: $(SERVER_MEM)

$(SERVER_MEM): $(SERVER_MEM_OBJS)
	@mkdir -p $(BIN)
	$(CXX) $^ -o $@

server_disc: $(SERVER_DISC)

$(SERVER_DISC): $(SERVER_DISC_OBJS)
	@mkdir -p $(BIN)
	$(CXX) $^ -o $@

$(OBJ)/mem_%.o: $(SRC)/%.cc
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS_MEM) -c $< -o $@

$(OBJ)/disc_%.o: $(SRC)/%.cc
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS_DISC) -c $< -o $@

## CLIENT:
CLIENT = $(BIN)/client
CLIENT_SRCS = \
	$(SRC)/connection.cc \
	$(SRC)/messageHandler.cc \
	$(SRC)/clientMessenger.cc \
	$(SRC)/client.cc
CLIENT_OBJS = $(patsubst $(SRC)/%.cc, $(OBJ)/%.o, $(CLIENT_SRCS))


client: $(CLIENT)

$(eval $(call build_executable,$(CLIENT),$(CLIENT_OBJS)))

ERR_CLIENT = $(BIN)/error_client
ERR_CLIENT_SRCS = \
	$(SRC)/connection.cc \
	$(SRC)/messageHandler.cc \
	$(SRC)/error_client.cc
ERR_CLIENT_OBJS = $(patsubst $(SRC)/%.cc, $(OBJ)/%.o, $(ERR_CLIENT_SRCS))

error_client: $(ERR_CLIENT)

$(eval $(call build_executable,$(ERR_CLIENT),$(ERR_CLIENT_OBJS)))

# Rule for creating object files
$(OBJ)/%.o: $(SRC)/%.cc
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -rf $(OBJ) $(BIN)

# Phony targets (clean, all)
.PHONY: all clean test_db client server