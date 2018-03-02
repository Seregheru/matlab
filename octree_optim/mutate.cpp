#include "octree.h"
#include <time.h>
#include <mex.h>

#define MAX_LEAVES 256
#define MAX_DEPTH 5

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    char * inchar = mxArrayToString(prhs[0]);
    string str(inchar);
    //load
    Octree * root = new Octree(0,-100,-100,100,100,100,0,1,0);
    //Octree * root = new Octree(0,0,0,255,255,255,0,1,0);
    root->String2Octree(str);
    //mutate
    list<Octree*> leaves; Octree* father;
    int i = 0; int nbleaves = root->getLeaves(leaves);
    double rd,weight;
    double sumweights = 0;
    Octree * child = 0;
    //choose child
    srand(time(NULL));
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
            if(rd<0.75)
            {
               child->split();
            }
            else
            {
                father = child->getFather();
                father->merge();
            }
        }
        else
        {
           father = child->getFather();
           father->merge();
        }
    }
    //save
    string strmutate = root->Octree2String();
    char * ouchar = (char*)mxCalloc((strmutate.size()+1),sizeof(char));
    copy(strmutate.begin(),strmutate.end(),ouchar);
    ouchar[strmutate.size()] = '\0';
    plhs[0] = mxCreateString(ouchar);
    //mexErrMsgTxt("stop!");
    delete root;
}



