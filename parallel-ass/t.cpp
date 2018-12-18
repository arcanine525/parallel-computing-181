#include <stdio.h>
#include <time.h>

typedef unsigned long long fuckin_large;

int main() {
  clock_t begin = clock();

  fuckin_large val = 0;
  for (fuckin_large j = 0; j < 10000000000; j++) {
    val ++; 
  }
  printf("Result ok: %llu\n", val);

  clock_t end = clock();
  double runtime = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Runtime: %lfs\n", runtime/1000);
}

