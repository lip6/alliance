dirs := $(dir $(shell echo */Makefile))

all:
	$(foreach dir, $(dirs), (cd $(dir) && make);)

clean:
	$(foreach dir, $(dirs), (cd $(dir) && make clean);)
