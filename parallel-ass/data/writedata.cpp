#include <iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <array>
#include <algorithm> 
typedef unsigned long long fuckin_large;

int arraySize(int arr[]){
	// count number of array
		// int num_items = rand() % 4 + 1;
		
		int count=0;
		// cout<<" so 0: "<<arr[0]<<" ,so 1: "<<arr[1]<<" ,so 2"<<arr[2];

		for(int i=0;i<26;i++){
			if(arr[i]!=-1){
				// cout<<arr[i];
				count++;
			}
			else{
				break;
			}
		}
		return count;
}



int main()
{
	clock_t begin = clock();
	char alphabets[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	char rString[20];
	fstream myfile;
	myfile.open("data.txt", ios::app);
	srand(time(NULL));
	cout << "Writting....";


	
	for (fuckin_large row = 0; row < 200000; row++)
	{
		// int duplicate[26];
		// for(int i=0;i<26;i++){
		// 	duplicate[i]=-1;
		// }
	// 	// // count number of array
		int num_items= rand()%26+1;
		int subset[26];
		for(int i=0;i<26;i++){
			subset[i]=-1;
		}
		for (int ri = 0; ri < num_items; ri++){
			int temp_num= rand()%26;
			int subset_count=arraySize(subset);
			// myfile << "size: "<<subset_count <<" ";	
			if(subset[0]==-1){
				subset[0]=temp_num;
			}
			else{
				int * p;
				p=std::find (subset, subset+26, temp_num);
				if(*p!=temp_num){
					subset[subset_count]=temp_num;
				}
			}
			// myfile << alphabets[temp_num] << " ";	
		}
		for(int i=0;i<26;i++){
			if(subset[i]==-1){
				break;
			}
			else{
				myfile << alphabets[subset[i]] << " ";	
			}
		}

	
		myfile << endl;

	}
	myfile.close();

	clock_t end = clock();
	double runtime = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Runtime: %lfs\n", runtime / 1000);
	return 0;
}
