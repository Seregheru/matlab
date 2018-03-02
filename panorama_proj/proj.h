#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265359

//DIM 3
struct Vec3 {
  double x;
  double y;
  double z;
  Vec3(double _x,double _y,double _z) : x(_x), y(_y), z(_z) {};
};

struct Mat3 {
  double data[9];
  //Mat3(_x0,_x1,_x2,_x3,_x4,_x5,_x6,_x7,_x8) {data[0]=_x0;data[1]=_x1;data[2]=_x2;
  //data[3]=_x3;data[4]=_x4;data[5]=_x5;data[6]=_x6;data[7]=_x7;data[8]=_x8;};
  Mat3(const double * d) { memcpy(data,d,sizeof(double)*9); };
};

// DIM 2
struct Vec2 {
  double x;
  double y;
  Vec2(double _x,double _y) : x(_x), y(_y) {};
};

struct Mat2 {
  double data[4];
  Mat2(double _xx,double _xy,double _yx,double _yy) { data[0] = _xx; data[1] = _xy; data[2] = _yx; data[3] = _yy; };
  Mat2(double * d) { memcpy(data,d,sizeof(double)*4); };
};

// DIM 3
double norm(Vec3 m);
double dot3(Vec3 * a, Vec3 * b);
Vec3 dot3m(Mat3 * M, Vec3 * a);
Mat3 multiply(Mat3 * A, Mat3 * B);

// DIM 2
double dot2(Vec2 a, Vec2 b);
Vec2 dot2m(Mat2 M, Vec2 a);

void spherical_proj(Mat3 * R, const int * I, const int h, const int w, const double f, const double * mask, double * res, double * resmask);
