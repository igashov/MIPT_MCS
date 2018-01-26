CC=gcc
SOURCE= main.c
CFLAGS= -std=gnu99 -fopenmp

BINS= main_O0 main_O1 main_O2 main_m64



all: $(BINS)

main_O0: main.c
	$(CC) $(CFLAGS) -O0 -o $@ $^

main_O1: main.c
	$(CC) $(CFLAGS) -O1 -o $@ $^

main_O2: main.c
	$(CC) $(CFLAGS) -O2 -o $@ $^

main_m64: main.c
	$(CC) $(CFLAGS) -O2 -march=native -o $@ $^

clean:
	rm -f $(BINS)
