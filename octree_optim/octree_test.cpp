#include "octree.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define MAX_DEPTH 5
#define MAX_LEAVES 256

using namespace std;

int main()
{
    srand(time(NULL));
    string s("()()()()()()()()");
    cout << "start\n";
    Octree * root = new Octree(0,0,0,100,100,100,0,1,0);
    root->String2Octree(s);
    //mutate
    for(int k = 0; k<100; k++)
    {
    list<Octree*> leaves; Octree* father;
    int i = 0; int nbleaves = root->getLeaves(leaves);
    cout << nbleaves <<"\n";
    double rd,weight;
    double sumweights = 0;
    Octree * child = 0;
    //choose child
    i = 0;
    for(list<Octree*>::iterator list_iter = leaves.begin(); list_iter != leaves.end(); list_iter++)
    {
        weight = (*list_iter)->getDepth();
        sumweights += weight;
    }
    rd = (double)rand() / RAND_MAX;
    weight = 0;
    for(list<Octree*>::iterator list_iter = leaves.begin(); list_iter != leaves.end(); list_iter++)
    {
        weight += (*list_iter)->getDepth()/sumweights;
        if(rd<=weight)
        {
            child = (*list_iter);
            break;
        }
    }
    //split or merge
    if(child->getFather()==root)
    {
	child->split();
    }
    else
    {
	if((child->getDepth()<MAX_DEPTH) && (nbleaves<MAX_LEAVES))
	{
	    rd = (double)rand() / RAND_MAX;
	    if(rd<0.5)
	    {
	       cout << "split\n";
	       child->split();
	    }
	    else
	    {
	        father = child->getFather();
	        cout << child->getDepth() << " merge\n";
	        father->merge();
	    }
	}
	else
	{
	   father = child->getFather();
	   cout << "merge\n";
	   father->merge();
	}
    }
    //save
    string strmutate = root->Octree2String();
    cout << strmutate << "\n";
    }
    delete root;
    return 0;
}
