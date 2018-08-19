
CXX=clang++
CXXFLAGS=-std=c++17 -Wall -O3

all: test

test:
	@$(MAKE) -C tests test

clean:
	@echo " CLEAN"
	@$(MAKE) -C tests clean

again: clean all

