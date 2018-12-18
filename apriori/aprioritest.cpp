/*----------------------------------------------------------------------
  File     : aprioritest.cpp
  Contents : apriori algorithm for finding frequent sets
  Author   : Bart Goethals
  Update   : 1/6/2003
----------------------------------------------------------------------*/

#include "AprioriSets.h"
#include <iostream>
#include <time.h>

int main(int argc, char *argv[])
{
  cout << "Apriori frequent itemset mining implementation" << endl;
  cout << "by Bart Goethals, 2000-2003" << endl;
  cout << "http://www.cs.helsinki.fi/u/goethals/" << endl << endl;
  
  if(argc < 4) {
    cerr << "usage: " << argv[0] << " datafile datatype minsup [output]" << endl;
    cerr << "datatype = 1 for Quest datagenerator binary" << endl;
    cerr << "datatype = 2 for Quest datagenerator ascii" << endl;
    cerr << "datatype = 3 for flat, i.e. all items per transaction on a single line" << endl;
    cerr << "datatype = 4 for ascii version of Quest datagenerator binary" << endl;
  }
  else {
    AprioriSets a;
    a.setVerbose(); // print information on nr of candidate itemsets etc
    a.setData(argv[1],atoi(argv[2]));

    a.setCountType(2); 
    // 1: to check k-subsets of transaction in set of candidates
    // 2: to check all candidates in transaction (default - best performance)

    a.setMinSup(atoi(argv[3]));
    if(argc==5) a.setOutputSets(argv[4]);
    
    clock_t start = clock();
    int sets = a.generateSets();
    cout << sets << "\t[" << (clock()-start)/double(CLOCKS_PER_SEC) << "s]" << endl;
    if(argc==5) cout << "Frequent sets written to " << argv[4] << endl;
  }

  return 0;
}

