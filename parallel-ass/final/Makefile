all: apriori.o transaction.o main.o
	gcc -std=gnu99 -fopenmp apriori.o transaction.o main.o -o main

clean:
	rm -rf *.o main
