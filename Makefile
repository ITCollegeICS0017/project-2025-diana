CXX=g++
CXXLAGS=-Wall -Wextra -Werror -std=c++17
SRC=$(wildcard src/*.cpp)
OBJ=$(SRC:.cpp=.o)
BIN=app

.PHONY: all run test clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN)

test: $(BIN) tests/test_basic.sh
	bash tests/test_basic.sh

clean:
	rm -f $(BIN) src/*.o
