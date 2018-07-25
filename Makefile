
CXX=clang++
CXXFLAGS=-std=c++17 -Wall -O3

BIN=test
SRC=test.cc

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(BIN)

again: clean all

