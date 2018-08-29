
CXX=clang++
CXXFLAGS=-std=c++17 -Wall -O3 -I..

BIN=ists proc

all: $(BIN)

%: %.cc
	@echo " CXX $^"
	@$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@echo " CLEAN"
	@rm -f $(BIN)

again: clean all

