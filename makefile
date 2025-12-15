CC = gcc
FLAGS = -std=c11 -Wall -Wextra

example: main.o spsc.o
	$(CC) ./build/*.o -o ./build/example

main.o: main.c spsc.h
	$(CC) $(FLAGS) -c main.c -o ./build/main.o

spsc.o: spsc.c spsc.h
	$(CC) $(FLAGS) -c spsc.c -o ./build/spsc.o

clean: 
	rm -f ./build/*
	@echo "Cleaned..."

run:
	make
	@echo "Running..."
	@echo " "
	./build/example
