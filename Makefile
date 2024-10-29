CFLAGS = -g
CC = clang

all: dining_philosophers

dining_philosophers: dp.o
	$(CC) $(CFLAGS) dp.o -o dining_philosophers
dp.o: dead_locked_code.c
	$(CC) $(CFLAGS) -c dead_locked_code.c -o dp.o
clean:
	rm -f dp.o dining_philosophers
