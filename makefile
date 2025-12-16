CC = gcc
FLAGS = -std=c11 -Wall -Wextra

example: main.o spsc.o mpsc.o
	$(CC) ./build/*.o -o ./build/example

main.o: main.c spsc.h mpsc.h
	$(CC) $(FLAGS) -c main.c -o ./build/main.o

spsc.o: spsc.c spsc.h
	$(CC) $(FLAGS) -c spsc.c -o ./build/spsc.o

mpsc.o: mpsc.c mpsc.h
	$(CC) $(FLAGS) -c mpsc.c -o ./build/mpsc.o

clean: 
	rm -f ./build/*
	@echo "Cleaned..."

run:
	make
	@echo "Running..."
	@echo " "
	./build/example
