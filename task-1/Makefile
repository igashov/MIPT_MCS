#
# Uncomment for debug and comment uncommented
#
CFLAGS= -g -std=c99 -Wall -Wextra
#CFLAGS= -O3 -std=c99 -Wall -Wextra
LIBS= -lrt -lpthread
CC= gcc

PROG=main

SOURCES=main.c\
	   	queue.c\
	   	thread_utils.c\
	   	longnum.c

all: $(PROG)

main: $(patsubst %.c, %.o , $(SOURCES))
	$(CC) -o $@ $^ $(LIBS)

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $<

run_test:
	valgrind --leak-check=full ./main random.txt

clean:
	rm -f $(PROG) $(patsubst %.c, %.o , $(SOURCES))

.PHONY:  run_test
