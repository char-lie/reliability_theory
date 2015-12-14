CC=colorgcc
CCFLAGS?=-Wall
CORE=randoms parameters estimates
EXPERIMENTS=experiment alpha
CORE_FILES=$(CORE:%=./core/%.c)
EXPERIMENT_FILES=$(EXPERIMENTS:%=./experiment/%.c)

all:
	@mkdir bin
	$(CC) -o bin/main $(CORE_FILES) $(EXPERIMENT_FILES) main.c -lm -O3 $(CCFLAGS)

debug: CCFLAGS+=-g
debug: all

