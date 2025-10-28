CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -Iinclude
LDFLAGS =

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(SRC_DIR)/%.o,$(SRC))
BIN = app
TEST_BIN = ticket_tests
TEST_SRC = tests/tests.cpp
INCLUDES = include

.PHONY: all run test clean

all: $(BIN)

# link application
$(BIN): $(OBJ)
    $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# compile rule for .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
    $(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(BIN)
    ./$(BIN)

# Build a test executable (keeps tests separate from main app)
# tests/tests.cpp should include or reference your test harness (see earlier instructions)
$(TEST_BIN): $(OBJ) $(TEST_SRC)
    $(CXX) $(CXXFLAGS) -o $@ $(filter-out $(SRC_DIR)/main.o,$(OBJ)) $(TEST_SRC)

test: $(TEST_BIN)
    ./$(TEST_BIN)

clean:
    rm -f $(BIN) $(TEST_BIN) $(SRC_DIR)/*.o
