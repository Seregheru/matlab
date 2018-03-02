#include "octree.h"
#include <mex.h>

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    char * inchar = mxArrayToString(prhs[0]);
    string str(inchar);
    const int * im = (int*)mxGetData(prhs[1]);
    const int * dims = mxGetDimensions(prhs[1]);
    int h = dims[1]; int w = dims[2];
    //mexPrintf("%d %d\n",h,w);
    //load
    Octree * root = new Octree(0,-100,-100,100,100,100,0,1,0);
    //Octree * root = new Octree(0,0,0,255,255,255,0,1,0);
    root->String2Octree(str);
    list<Octree*> leaves;
    int nbleaves = root->getLeaves(leaves); 
    int i=0;
    //mexErrMsgTxt("stop!\n");
    for(list<Octree*>::iterator list_iter = leaves.begin(); list_iter != leaves.end(); list_iter++)
    {
            (*list_iter)->setIndex(i);
            i++;
    }
    //loop over pixels 
    plhs[0] = mxCreateNumericMatrix(h,w,mxINT32_CLASS,mxREAL);
    int * res = (int*)mxGetPr(plhs[0]);
    Point p; int k;
    for(i=0;i<h*w;i++)
    {
        k = 3*i;
        p = Point((double)im[k],(double)im[k+1],(double)im[k+2]);
        res[i] = root->search(p);
    }
    //mexErrMsgTxt("stop!\n");
    plhs[1] = mxCreateDoubleScalar((double)nbleaves);
}