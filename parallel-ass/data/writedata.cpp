#include <iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>
#include <fstream>
typedef unsigned long long fuckin_large;
int main()
{
	clock_t begin = clock();
	char alphabets[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	char rString[20];
	fstream myfile;
	myfile.open("data.txt", ios::app);
	srand(time(NULL));
	cout << "Writting....";
	for (fuckin_large i = 0; i < 200000; i++)
	{

		// cout << i << endl;
		int num_items = rand() % 4 + 1;
		for (int j = 0; j < num_items; j++)
		{
			int temp = rand() % 26;
			myfile << alphabets[temp] << " ";
		}
		myfile << endl;
	}
	myfile.close();

	clock_t end = clock();
	double runtime = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Runtime: %lfs\n", runtime / 1000);
	return 0;
}
