
all: test

test:
	@$(MAKE) -C tests test

clean:
	@$(MAKE) -C tests clean
	@$(MAKE) -C examples clean

again: clean all

