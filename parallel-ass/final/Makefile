all: apriori.o transaction.o main.o
	gcc -fopenmp apriori.o transaction.o main.o -o main

clean:
	rm -rf *.o main
