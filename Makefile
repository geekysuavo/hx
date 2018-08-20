
all: test

test:
	@$(MAKE) -C tests test

clean:
	@echo " CLEAN"
	@$(MAKE) -C tests clean

again: clean all

