#include "proj.h"
#include <mex.h>

double norm(Vec3 * m)
{
  return sqrt(m->x*m->x+m->y*m->y+m->z*m->z);
}

double dot3(Vec3 * a, Vec3 * b)
{
  return a->x*b->x+a->y*b->y+a->z*b->z;
}

Vec3 dot3m(Mat3 * M, Vec3 * a)
{
  return Vec3(M->data[0]*a->x+M->data[1]*a->y+M->data[2]*a->z,M->data[3]*a->x+M->data[4]*a->y+M->data[5]*a->z,M->data[6]*a->x+M->data[7]*a->y+M->data[8]*a->z);
}

Mat3 multiply(Mat3 * A, Mat3 * B)
{
  double tmp[9];
  tmp[0] = A->data[0]*B->data[0]+A->data[1]*B->data[3]+A->data[2]*B->data[6];
  tmp[1] = A->data[0]*B->data[1]+A->data[1]*B->data[4]+A->data[2]*B->data[7];
  tmp[2] = A->data[0]*B->data[2]+A->data[1]*B->data[5]+A->data[2]*B->data[8];
  tmp[3] = A->data[3]*B->data[0]+A->data[4]*B->data[3]+A->data[5]*B->data[6];
  tmp[4] = A->data[3]*B->data[1]+A->data[4]*B->data[4]+A->data[5]*B->data[7];
  tmp[5] = A->data[3]*B->data[2]+A->data[4]*B->data[5]+A->data[5]*B->data[8];
  tmp[6] = A->data[6]*B->data[0]+A->data[7]*B->data[3]+A->data[8]*B->data[6];
  tmp[7] = A->data[6]*B->data[1]+A->data[7]*B->data[4]+A->data[8]*B->data[7];
  tmp[8] = A->data[6]*B->data[2]+A->data[7]*B->data[5]+A->data[8]*B->data[8];
  return Mat3(tmp);
}

double dot2(Vec2 a, Vec2 b)
{
  return a.x*b.x+a.y*b.y;
}

Vec2 dot2m(Mat2 M, Vec2 a)
{
  return Vec2(M.data[0]*a.x+M.data[1]*a.y,M.data[2]*a.x+M.data[3]*a.y);
}

void spherical_proj(Mat3 * R, const double * I, const int h, const int w, const double f, const double * mask, double * res, double * resmask)
{

  double s1 = h/180.; double s2 = w/360.;
  double n, phi,lambda, phi_, lambda_;
  int p1,p2,l1,l2;
  for(int i=0;i<480;i++)
  {
    for(int j=0;j<640;j++)
    {
      Vec3 M(f,j-320,i-240);
      Vec3 Mt = dot3m(R,&M);
      n = norm(&Mt);
      phi = atan2(Mt.z/n,sqrt(Mt.x*Mt.x+Mt.y*Mt.y)/n)*180/PI;
      lambda = atan2(Mt.y/n,Mt.x/n)*180/PI;
      phi_ = fmod(phi*s1+h/2.,h);
      lambda_ = fmod(lambda*s2+w/2.,w);
      p1 = (int)floor(phi_); p2 = (p1+1) % h;
      l1 = (int)floor(lambda_); l2 = (l1+1) % w;
      Vec2 gg(p2-phi_,phi_-p1); Vec2 hh(l2-lambda_,lambda_-l1);
      Mat2 r(I[3*(h*l1+p1)],I[3*(h*l2+p1)],I[3*(h*l1+p2)],I[3*(h*l2+p2)]);
      Mat2 g(I[3*(h*l1+p1)+1],I[3*(h*l2+p1)+1],I[3*(h*l1+p2)+1],I[3*(h*l2+p2)+1]);
      Mat2 b(I[3*(h*l1+p1)+2],I[3*(h*l2+p1)+2],I[3*(h*l1+p2)+2],I[3*(h*l2+p2)+2]);
      res[3*(480*j+i)] = dot2(gg,dot2m(r,hh));
      res[3*(480*j+i)+1] = dot2(gg,dot2m(g,hh));
      res[3*(480*j+i)+2] = dot2(gg,dot2m(b,hh));
      resmask[480*j+i] = mask[h*l1+p1];
      //mexPrintf("test : %f \n",test);
      //mexErrMsgTxt("test");
    }
  }

}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

    // INPUT VARIABLES

    const double * r = (double *)mxGetData(prhs[0]);
    const double * I = (double *)mxGetData(prhs[1]);
    const int h = (int)mxGetScalar(prhs[2]);
    const int w = (int)mxGetScalar(prhs[3]);
    const double f = (double)mxGetScalar(prhs[4]);
    const double * mask = (double *)mxGetData(prhs[5]);

    // AUXILIARY VARIABLES

    Mat3 R = Mat3(r);
    int n = 640*480;
    /*
    double* res = new double[3*n];
    memset(res,0,sizeof(double)*3*n);
    double* resmask = new double[n];
    memset(resmask,0,sizeof(double)*n);
    */

    int dim[3] = {3,480,640};
    plhs[0] = mxCreateNumericArray(3,dim,mxDOUBLE_CLASS,mxREAL);
    double * res = (double*)mxGetPr(plhs[0]);
    memset(res,0,sizeof(double)*3*n);

    plhs[1] = mxCreateNumericMatrix(480,640,mxDOUBLE_CLASS,mxREAL);
    double * resmask = (double*)mxGetPr(plhs[1]);
    memset(resmask,0,sizeof(double)*n);

    spherical_proj(&R, I, h, w, f, mask, res, resmask);

    // OUPUT VARIABLES
    /*
    int dim[3] = {3,960,1280};
    plhs[0] = mxCreateNumericArray(3,dim,mxDOUBLE_CLASS,mxREAL);
    double * tmpd_0 = (double*)mxGetPr(plhs[0]);
    memcpy(tmpd_0,res,sizeof(double)*3*n);

    plhs[1] = mxCreateNumericMatrix(960,1280,mxDOUBLE_CLASS,mxREAL);
    double * tmpd_1 = (double*)mxGetPr(plhs[1]);
    memcpy(tmpd_1,resmask,sizeof(double)*n);

    delete[] res;
    delete[] resmask;
    */

}
