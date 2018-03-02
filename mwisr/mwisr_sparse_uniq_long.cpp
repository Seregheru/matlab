#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include <utility>
#include <list>
#include <mex.h>

#define p1x xl[i]
#define p2x xl[i]
#define p3x xr[i]
#define p4x xr[i]
#define p1y yt[j]
#define p2y yb[j]
#define p3y yt[j]
#define p4y yb[j]
//#define N 1000000

using namespace std;

struct Point {
 int x;
 int y;
 Point() : x(0),y(0) {} 
 Point(int _x, int _y) : x(_x), y(_y) {}
 void set(int _x, int _y) {x=_x;y=_y;}
};

struct Datum {
  unsigned long int v0;
  unsigned long int v1;
  int index;
  Datum() : v0(0),v1(0),index(0) {} 
  Datum(unsigned long int _v0,unsigned long int _v1, int _index) : v0(_v0),v1(_v1),index(_index) {}
  void set(unsigned long int _v0,unsigned long int _v1, int _index) {v0=_v0;v1=_v1;index=_index;}
  friend bool operator<(Datum a, Datum b) { return ((a.v1==b.v1) ? (a.v0<b.v0) : (a.v1<b.v1)) ; }
  friend bool operator==(Datum a, Datum b) { return ((a.v1==b.v1) && (a.v0==b.v0)); }
};

pair<long unsigned int,long unsigned int> hash_func(list<int>& line)
{
    pair<long unsigned int,long unsigned int> sum(0,0);
    for(list<int>::iterator iter = line.begin(); iter != line.end(); iter++)
    {
        if(*iter<64)
            sum.first += (1 << (*iter));
        else
            sum.second += (1 << (*iter));
    }
    return sum;// % N;
}

pair<int,int> generate_mwisr(const int * xl, const int * xr, const int * yt, const int * yb, int n, list<Point>& a)
{
    int i,j,k,index,lengtha;
    lengtha = 0;
    list<int>* lines = new list<int>[4*n*n];
    for(k=0;k<n;k++)
    {
      index = 0;
      for(i=0;i<n;i++)
      {
        for(j=0;j<n;j++)
        {
          if( (xl[k] <= p1x) && (p1x <= xr[k]) && (yt[k] <= p1y) && (p1y <= yb[k]) )
          {
            lengtha++;
            lines[index].push_back(k);
          }
          index++;
          if( (xl[k] <= p2x) && (p2x <= xr[k]) && (yt[k] <= p2y) && (p2y <= yb[k]) )
          {
            lengtha++;
            lines[index].push_back(k);
          }
          index++;
          if( (xl[k] <= p3x) && (p3x <= xr[k]) && (yt[k] <= p3y) && (p3y <= yb[k]) )
          {
            lengtha++;
            lines[index].push_back(k);
          }
          index++;
          if( (xl[k] <= p4x) && (p4x <= xr[k]) && (yt[k] <= p4y) && (p4y <= yb[k]) )
          {
            lengtha++;
            lines[index].push_back(k);
          }
          index++;
        }
      }
    }
    vector<Datum> hash_lines(4*n*n);
    pair<unsigned long int,unsigned long int> hash_res;
    for(i=0;i<4*n*n;i++)
    {
        hash_res = hash_func(lines[i]);
        hash_lines[i].v0 = hash_res.first;
        hash_lines[i].v1 = hash_res.second;
        hash_lines[i].index = i;
    }
    sort(hash_lines.begin(),hash_lines.end());
    vector<Datum>::iterator last = unique(hash_lines.begin(),hash_lines.end());
    int nbunique = distance(hash_lines.begin(),last);
    list<int> line;
    list<Point> klines[n];
    for(i=0;i<nbunique;i++)
    {
        line = lines[hash_lines[i].index];
        for(list<int>::iterator iter = line.begin(); iter != line.end(); iter++)
        {
            klines[*iter].push_back(Point(i,*iter));
        }
    }
    for(k=0;k<n;k++)
        a.splice(a.end(),klines[k]);
    delete [] lines;
    pair<int,int> res(lengtha,nbunique);
    return res;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

    const int * rects = (int *)mxGetData(prhs[0]);
    const size_t * dims = mxGetDimensions(prhs[0]);
    int n = dims[0];
    const int * xl = &(rects[0]);
    const int * yt = &(rects[n]);
    const int * xr = &(rects[2*n]);
    const int * yb = &(rects[3*n]);
    list<Point> ptdata;
    pair<int,int> nb = generate_mwisr(xl,xr,yt,yb,n,ptdata);
    plhs[0] = mxCreateSparse(nb.second,n,nb.first,mxREAL);
    double * pr = (double *)mxGetPr(plhs[0]);
    size_t * ir = (size_t *)mxGetIr(plhs[0]);
    size_t * jc = (size_t *)mxGetJc(plhs[0]);
    memset(jc,0,(n+1)*sizeof(size_t));
    int c = 0; int column = -1;
    for(list<Point>::iterator list_iter = ptdata.begin(); list_iter != ptdata.end(); list_iter++)
    {
        ir[c] = list_iter->x;
        pr[c] = 1;
        if( list_iter->y > column ) 
        { 
            jc[list_iter->y] = c;
            column = list_iter->y;
        }
        c++;
    }
    for(int k=0;k<n;k++)
    {
        if(jc[k+1]<jc[k])
            jc[k+1] = jc[k]+1;
    }
    jc[n+1] = nb.first;
}
