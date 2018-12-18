/*----------------------------------------------------------------------
  File     : AprioriSets.h
  Contents : apriori algorithm for finding frequent sets
  Author   : Bart Goethals
  Update   : 1/6/2003 
----------------------------------------------------------------------*/

#include <set>
#include <vector>
#include <fstream>
using namespace std;
#include "Data.h"
#include "Item.h"

class Element
{
public:
	Element(int iold, int inew=0) : oldid(iold), id(inew){}

	int oldid;
	int id;
	bool operator< (const Element  &e) const {return oldid < e.oldid;}
};

class AprioriSets 
{
public:

	AprioriSets();
	~AprioriSets();

	void setData(char *fn, int type);
	int setOutputSets(char *fn);
	void setMinSup(int ms){minsup=ms;}
	int generateSets();

	void setVerbose(){verbose=true;}
	void setCountType(int t){countType=t;}

protected:
	
	int generateCandidates(int level);
	int generateCandidates(int level, set<Item> *items, int depth, int *current);
	bool checkSubsets(int sl, int *iset, set<Item> *items, int spos, int depth);

	int countCandidates(int level);

	// test k-subsets of transaction that are candidate itemsets
	int processTransaction(int level, Transaction *t, set<Item> *items, int spos, int depth);
	// test all candidates whether contained in transaction
	int processTransaction2(int level, Transaction *t, set<Item> *items, int spos, int depth);

	void ReOrder();

	int pruneNodes(int level);
	int pruneNodes(int level, set<Item> *items, int depth);

	int pruneCandidates(int level);
	int pruneCandidates(int level, set<Item> *items, int depth, int *itemset);

	void printSet(const Item& item, int *itemset, int length);


private:

	Item *trie;

	int minsup;

	ofstream setsout;

	Data *data;
	int *remap;
	set<Element> *relist;

	bool verbose;

	int countType;
};
