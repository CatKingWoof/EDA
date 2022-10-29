/*
* FILENAME: transfer.c
* Overview: BDD tutorial
* AUTHOR: David Kebo Houngninou
*/

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "cudd.h"
#include<fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


/**
 * Writes a dot file representing the argument DDs
 * @param the node object
 */
void write_dd (DdManager *gbm, DdNode *dd, char* filename)
{
    FILE *outfile; // output file pointer for .dot file
    outfile = fopen(filename,"w");
    DdNode **ddnodearray = (DdNode**)malloc(sizeof(DdNode*)); // initialize the function array
    ddnodearray[0] = dd;
    Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile); // dump the function to .dot file
    free(ddnodearray);
    fclose (outfile); // close the file */
}

// This program creates a single BDD variable
int main (int argc, char *argv[])
{
	std::ifstream ifs;
    std::ofstream ofs;
	ifs.open(argv[1]);
	ofs.open(argv[2]);
    DdManager *gbm; /* Global BDD manager. */
    char filename[30];
	char c;
	DdNode *bdd,*bdd2,*tmp, *tmp_neg,*var;
    gbm = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0); /* Initialize a new BDD manager. */
    bdd = Cudd_ReadOne(gbm);
	Cudd_Ref(bdd);
	ifs>>c;
	while(c!='.'&&c!='+')
	{
		if(c>96)//小寫
		{
			var=Cudd_bddIthVar(gbm,c-97);
			tmp = Cudd_bddAnd(gbm, var, bdd);
			Cudd_Ref(tmp);
			Cudd_RecursiveDeref(gbm, bdd);
			bdd = tmp;
		}
		else
		{
			var = Cudd_bddIthVar(gbm, c - 65);
			tmp_neg = Cudd_Not(var); 
			tmp = Cudd_bddAnd(gbm, tmp_neg, bdd); 
			Cudd_Ref(tmp);
			Cudd_RecursiveDeref(gbm, bdd);
			bdd = tmp;
		}
		ifs>>c;
	}
			
	
	while(c!='.')
	{
		bdd2 = Cudd_ReadOne(gbm);
		Cudd_Ref(bdd2);
		ifs>>c;

		while(c!='+'&&c!='.')
		{
			if(c>96)//小寫
			{
				var=Cudd_bddIthVar(gbm,c-97);
				tmp = Cudd_bddAnd(gbm, var, bdd2);
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(gbm, bdd2);
				bdd2 = tmp;
			}
			else
			{
				var = Cudd_bddIthVar(gbm, c - 65);
				tmp_neg = Cudd_Not(var); 
				tmp = Cudd_bddAnd(gbm, tmp_neg, bdd2); 
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(gbm, bdd2);
				bdd2 = tmp;
			}
		ifs>>c;
		}
		bdd = Cudd_bddOr(gbm, bdd, bdd2);
		Cudd_Ref(bdd);
	}
	ifs>>c;
	vector <int> order;
	int t,sol,best_sol=999999;
	int* tt;
	
	while(!ifs.eof())
	{
		while(c!='.')
		{
			t=int(c)-97;
			
			order.push_back(t);
			ifs>>c;
		}
		tt= new int[order.size()];
		for(int t=0;t<order.size();t++)
		{
			tt[t]=order[t];
		}
		Cudd_ShuffleHeap(gbm,tt);
		order.clear();
		if(Cudd_DagSize(bdd)>1)
		{
			sol=Cudd_DagSize(bdd)+1;
		}
		else
		{
			sol=Cudd_DagSize(bdd);
		}
		if(sol<best_sol)
		{
			best_sol=sol;
		}
		delete []tt;
		ifs>>c;
	}
		
		
	ofs<<best_sol;	
	
    sprintf(filename, "lab3.dot"); /*Write .dot filename to a string*/
    write_dd(gbm, bdd, filename);  /*Write the resulting cascade dd to a file*/
    Cudd_Quit(gbm);
    return 0;
}