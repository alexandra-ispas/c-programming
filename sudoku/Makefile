build: cJSON.o

run:
	./sudoku

build: cJSON.o solver.o
	gcc -Wall -o sudoku cJSON.o solver.o 

cJSON.o: cJSON.c
	gcc -c cJSON.c

solver.o: solver.c
	gcc -c solver.c 

.PHONY: clean
clean:
	rm -rf *.o sudoku