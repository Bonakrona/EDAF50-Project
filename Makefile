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
# Macro for building the executable
define build_executable
$1: $2
	@mkdir -p $(BIN)
	$(CXX) $2 -o $1
endef

## ALL:
all: server client

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

test_db: $(TEST_DB)

$(eval $(call build_executable,$(TEST_DB),$(TEST_DB_OBJS)))

## SERVER:
SERVER_IN_MEMORY = $(BIN)/server_mem
SERVER_IN_DISC = $(BIN)/server_disc

SERVER_SRCS_MEM = \
	$(SRC)/connection.cc \
	$(SRC)/server.cc \
	$(SRC)/newsapp.cc \
	$(SRC)/messageHandler.cc \
	$(SRC)/article.cc \
	$(SRC)/newsgroup.cc \
	$(SRC)/inMemory.cc \
	$(SRC)/servermain_mem.cc
SERVER_OBJS_MEM = $(patsubst $(SRC)/%.cc, $(OBJ)/%.o, $(SERVER_SRCS_MEM))

SERVER_SRCS_DISC = \
	$(SRC)/connection.cc \
	$(SRC)/server.cc \
	$(SRC)/newsapp.cc \
	$(SRC)/messageHandler.cc \
	$(SRC)/article.cc \
	$(SRC)/newsgroup.cc \
	$(SRC)/InDisc.cc \
	$(SRC)/servermain_disc.cc
SERVER_OBJS_DISC = $(patsubst $(SRC)/%.cc, $(OBJ)/%.o, $(SERVER_SRCS_DISC))

server: $(SERVER_IN_MEMORY) $(SERVER_IN_DISC) 

$(eval $(call build_executable,$(SERVER_IN_MEMORY),$(SERVER_OBJS_MEM)))
$(eval $(call build_executable,$(SERVER_IN_DISC),$(SERVER_OBJS_DISC)))


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