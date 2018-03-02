#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mex.h>

/////////////
//constants//
/////////////
#define RING_COLOR 0.3

#if !defined(MAX)
#define    MAX(A, B)    ((A) > (B) ? (A) : (B))
#endif

#if !defined(MIN)
#define    MIN(A, B)    ((A) < (B) ? (A) : (B))
#endif

/////////////
//utilities//
/////////////

struct MySize
{
	int h;
	int w;
    MySize() : h(0), w(0) {}
	MySize(int h_i, int w_i) : h(h_i), w(w_i) {}
};

/////////////////////
//Wavelets sparsity//
/////////////////////

double integral_rect(int * rect, const double * intim, int h )
{
	return intim[(rect[3]+1)*h+rect[2]+1] - intim[(rect[3]+1)*h+rect[0]] - intim[rect[1]*h+rect[2]+1] + intim[rect[1]*h+rect[0]];
}

double color_contrast(int * inside, const double ** colorint, double * histrec, double * histring, int h, int * ring, const int BINS)
{
    
    memset(&(histrec[0]),0,sizeof(double)*BINS);
    for(int i=0;i<BINS;i++)
        histrec[i] = integral_rect(&(inside[0]),&(colorint[i][0]),h);
    memset(&(histring[0]),0,sizeof(double)*BINS);
    for(int i=0;i<BINS;i++)
        histring[i] = integral_rect(&(ring[0]),&(colorint[i][0]),h);
    //mexPrintf("Value is %g %g %g \n",histring[0], histring[100], histring[200]);
    double sum_inside = 0; double sum_ring = 0;
    for(int i=0;i<BINS;i++)
    {
        sum_inside += histrec[i];
        sum_ring += histring[i];
    }
    
    double res = 0; double tmp;
    for(int i=0;i<BINS;i++)
    {
        histrec[i] = histrec[i]/sum_inside;
        histring[i] = (histring[i]-histrec[i])/(sum_ring-sum_inside);
        if(histrec[i]+histring[i]>0)
        {
            tmp = (histring[i]-histrec[i]);
            res = res + tmp*tmp/(histrec[i]+histring[i]);
        }
    }
    //mexPrintf("Value is %g \n",res);
    //return res;
    return 1.-exp(-res/0.6);

}

void facade_detector(const double ** colorint, MySize img_size, const double * mask, double * res, const int * xmin, const int * ymin, const int * xmax, const int * ymax, int n, const int BINS)
{
    
    // COLOR CONTRAST
	
    int rec[4];  int ringrec[4];
    int hw, ww, ringh,ringw;

    double histrec[BINS];
    double histring[BINS];
    
    for(int i=0;i<n;i++)
    {
        rec[0] = xmin[i]-1; rec[1] = ymin[i]-1; rec[2] = xmax[i]-1; rec[3] = ymax[i]-1;
        hw = xmax[i] - xmin[i];
        ww = ymax[i] - ymin[i];
        ringh = (int)floor(RING_COLOR*(double)hw);
        ringw = (int)floor(RING_COLOR*(double)ww);
        ringrec[0] = xmin[i]>ringh ? xmin[i]-ringh : 0;
        ringrec[1] = ymin[i]>ringw ? ymin[i]-ringw : 0;
        ringrec[2] = xmax[i]<img_size.h-ringh ? xmax[i]+ringh-1 : img_size.h-2;
        ringrec[3] = ymax[i]<img_size.w-ringw ? ymax[i]+ringw-1 : img_size.w-2;
        if((ringh>0) && (ringw>0) && (integral_rect(&(rec[0]),&(mask[0]),img_size.h)/(hw*ww)>0.66))
        {
            res[i] = color_contrast( &(rec[0]), colorint, &(histrec[0]), &(histring[0]), img_size.h, &(ringrec[0]), BINS );
        }
    }
    
}



void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

	// INPUT VARIABLES
    
	
    const mxArray * cellcolor = prhs[0];
    const int * dims = mxGetDimensions(prhs[0]);
    const int BINS = dims[0];
	const double* colorint[BINS];
    mxArray * cellElement;
	for (int i=0; i<BINS; i++) 
    {
		cellElement = mxGetCell(cellcolor,i);
		colorint[i] = (double*)mxGetPr(cellElement);
	}
    const double * matrix_size = (double *)mxGetData(prhs[1]);
    
    const double * mask = (double *)mxGetData(prhs[2]);
    
    const int n = (int)mxGetScalar(prhs[3]);
    const int * xmin = (int *)mxGetData(prhs[4]);
    const int * ymin = (int *)mxGetData(prhs[5]);
    const int * xmax = (int *)mxGetData(prhs[6]);
    const int * ymax = (int *)mxGetData(prhs[7]);
    
    // AUXILIARY VARIABLES
    
    MySize img_size;
    img_size.h = (int)floor(matrix_size[0])+1; img_size.w = (int)floor(matrix_size[1])+1;
    
 	plhs[0] = mxCreateNumericMatrix(n,1,mxDOUBLE_CLASS,mxREAL);
    double * res = (double*)mxGetPr(plhs[0]);
    memset(res,0,sizeof(double)*n);
    
    // MAIN PROGRAM

	facade_detector(colorint, img_size, &(mask[0]), res, xmin, ymin, xmax, ymax, n, BINS);

}
