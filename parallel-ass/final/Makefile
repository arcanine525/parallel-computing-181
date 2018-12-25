all: apriori.o transaction.o main.o
	gcc -fopenmp -std=c99 apriori.o transaction.o main.o -o main

clean:
	rm -rf *.o main
