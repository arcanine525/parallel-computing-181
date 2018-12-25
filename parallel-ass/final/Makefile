all: apriori.o transaction.o main.o
	gcc -std=c99 -fopenmp apriori.o transaction.o main.o -o main

clean:
	rm -rf *.o main
