#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mex.h>

#define EPS 10e-8

// class Mesh()
// {
// private:
//     std::vector<Mesh*> succ;
// public:
//     double * V0;
//     double * V1;
//     double * V2;
//     Mesh() : V0(0), V1(0), V2(0) {};
//     Mesh(double * v0, double * v1, double * v2) : V0(v0), V1(v1), V2(v2) {};
//     std::vector<Mesh*> getsucc();
//     void addsucc(Mesh * m);
// }
// 
// std::vector<Mesh*> Mesh::getsucc()
// {
//     return succ;
// }
// 
// void Mesh::addsucc(Mesh * m)
// {
//     succ.push_back(m);
// }

inline double dot(double * a, double * b)
{
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

int intersect3D_RayTriangle( double * dir, double  * V0, double * V1, double * V2 )
{
    double u[3]; double v[3]; double n[3];              // triangle vectors
    double w0[3]; double w[3];           // ray vectors
    double  r, a, b;              // params to calc ray-plane intersect
    double I[3]; //intersection

    // get triangle edge vectors and plane normal
    u[0] = V1[0] - V0[0]; u[1] = V1[1] - V0[1]; u[2] = V1[2] - V0[2];
    v[0] = V2[0] - V0[0]; v[1] = V2[1] - V0[1]; v[2] = V2[2] - V0[2];
    n[0] = u[1]*v[2] - u[2]*v[1]; n[1] = u[2]*v[0] - u[0]*v[2]; n[2] = u[0]*v[1] - u[1]*v[0]; // cross product
    if ((n[0] == 0)&&(n[1] == 0)&&(n[2] == 0))             // triangle is degenerate
        return -1;                  // do not deal with this case

    w0[0] = -V0[0]; w0[1] = -V0[1]; w0[2] = -V0[2];
    a = -dot(n,w0);
    b = dot(n,dir);
    if (fabs(b) < EPS) {     // ray is  parallel to triangle plane
        if (a == 0)                 // ray lies in triangle plane
            return 2;
        else return 0;              // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                    // ray goes away from triangle
        return 0;                   // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    I[0] = r * dir[0]; I[1] = r * dir[1]; I[2] = r * dir[2];            // intersect point of ray and plane

    // is I inside T?
    double    uu, uv, vv, wu, wv, D;
    uu = dot(u,u);
    uv = dot(u,v);
    vv = dot(v,v);
    w[0] = I[0] - V0[0]; w[1] = I[1] - V0[1]; w[2] = I[2] - V0[2];
    wu = dot(w,u);
    wv = dot(w,v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    double s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)         // I is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return 0;

    return 1;                       // I is in T
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

    double * dir = (double *)mxGetData(prhs[0]);
	const int p = (int)mxGetScalar(prhs[1]);
    
    const int K = (int)mxGetScalar(prhs[2]);
    
    const mxArray * cell = prhs[3];
	const int * dims = mxGetDimensions(prhs[3]);
	const double* faces[K];

	mxArray * cellElement;
	for (int i=0; i<K; i++) 
    {
		cellElement = mxGetCell(cell,i);
		faces[i] = (double*)mxGetPr(cellElement);
	}
    
    const mxArray * cellint = prhs[4];
    int* indices[K-1];
    mxArray * cellElementint;
    for (int i=0; i<K-1; i++) 
    {
		cellElementint = mxGetCell(cellint,i);
		indices[i] = (int*)mxGetPr(cellElementint);
	}
    
    const int * ressize = (int *)mxGetData(prhs[5]);
    
    plhs[0] = mxCreateNumericMatrix(ressize[K-1],1,mxDOUBLE_CLASS,mxREAL);
    double *ptres = (double*)mxGetPr(plhs[0]);
    memset(ptres,0,sizeof(double)*ressize[K-1]);
    
    int inter = 0; int k, l, m, facesize, index;
    double dirj[3]; double V0[3]; double V1[3]; double V2[3];
    int maxinter = 0; int listsucc[4];
    
    for(int j=0;j<p;j++)
    {
        dirj[0] = dir[3*j]; dirj[1] = dir[3*j+1]; dirj[2] = dir[3*j+2];
        
        for(int i=0;i<ressize[0];i++)
        {
            facesize = ressize[0]*3;
            k = 3*i; l = k+facesize; m = l+facesize;
            V0[0] = faces[0][k]; V0[1] = faces[0][k+1]; V0[2] = faces[0][k+2];
            V1[0] = faces[0][l]; V1[1] = faces[0][l+1]; V1[2] = faces[0][l+2];
            V2[0] = faces[0][m]; V2[1] = faces[0][m+1]; V2[2] = faces[0][m+2];
            
            inter = intersect3D_RayTriangle(dirj,V0,V1,V2);
            if(inter==1)
            {
                memcpy(&(listsucc[0]),&(indices[0][4*i]),sizeof(int)*4);
                break;
            }
        }

        for(int q=1;q<K;q++)
        {
            for(int i=0;i<4;i++)
            {
                index = listsucc[i];
                facesize = ressize[q]*3;
                k = 3*index; l = k+facesize; m = l+facesize;
                V0[0] = faces[q][k]; V0[1] = faces[q][k+1]; V0[2] = faces[q][k+2];
                V1[0] = faces[q][l]; V1[1] = faces[q][l+1]; V1[2] = faces[q][l+2];
                V2[0] = faces[q][m]; V2[1] = faces[q][m+1]; V2[2] = faces[q][m+2];

                inter = intersect3D_RayTriangle(dirj,V0,V1,V2);
                if(inter==1)
                {
                    if(q<K-1)
                    {
                        memcpy(&(listsucc[0]),&(indices[q][4*index]),sizeof(int)*4);
                    }
                    else
                    {
                        ptres[index]++;
                        maxinter++;
                    }
                    break;
                }
            }
        }

    }
    
    for(int i=0;i<ressize[K-1];i++)
        ptres[i] = ptres[i]/maxinter;
    
}