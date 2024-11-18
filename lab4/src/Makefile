CC=gcc
CFLAGS=-I. -m32

all: parallel_min_max_2 zombie process_memory psum

parallel_min_max_2: parallel_min_max_2.c utils_2.o find_min_max_2.o
	$(CC) -o parallel_min_max_2 parallel_min_max_2.c utils_2.o find_min_max_2.o $(CFLAGS)

utils_2.o: utils_2.c utils_2.h
	$(CC) -o utils_2.o -c utils_2.c $(CFLAGS)

find_min_max_2.o: find_min_max_2.c find_min_max_2.h
	$(CC) -o find_min_max_2.o -c find_min_max_2.c $(CFLAGS)

zombie: zombie.c
	$(CC) -o zombie zombie.c $(CFLAGS)

process_memory: process_memory.c
	$(CC) -o process_memory process_memory.c $(CFLAGS)

psum: psum.c utils_2.o sum.o
	$(CC) -o psum psum.c utils_2.o sum.o -lpthread $(CFLAGS)

sum.o: sum.c sum.h
	$(CC) -o sum.o -c sum.c $(CFLAGS)

clean:
	rm -f *.o parallel_min_max_2 zombie process_memory psum