
CXX=clang++
CXXFLAGS=-std=c++17 -Wall -O3
CXXFLAGS+= -fsanitize=address
CXXFLAGS+= -ftemplate-depth=2048

GEN=cxxtestgen
GENFLAGS=--error-printer

TEST=array dims fft index matrix scalar schedule trig vector
SRC=$(addsuffix .cc,$(TEST))
HDR=$(addsuffix .hh,$(TEST))

.SUFFIXES: .hh .cc

.PRECIOUS: %.hh %.cc

all: $(TEST)

%: %.cc
	@echo " CXX $^"
	@$(CXX) $(CXXFLAGS) $^ -o $@

%.cc: %*.hh
	@echo " GEN $@"
	@$(GEN) $(GENFLAGS) -w $(@:%.cc=%) -o $@ $^

clean:
	@echo " CLEAN"
	@rm -f $(SRC) $(TEST)

again: clean all

test: all
	@echo " TEST"
	@for t in $(TEST); do ./$$t; done

