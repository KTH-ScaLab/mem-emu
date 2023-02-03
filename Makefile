CC ?= gcc
CFLAGS ?= -O2 -g

all: emu agg

emu: emu.c
	$(CC) $(CFLAGS) -o $@ $^ -lrt -lnuma -lutil

agg: agg.c
	$(CC) $(CFLAGS) -o $@ $^ -lm
