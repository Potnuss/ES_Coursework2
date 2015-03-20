/* 
 * ES Coursework Part 2, created by Pontus Petersson, March 2015
 * Embedded Systems, University of Edinburgh
 * Digital filter designed by mkfilter/mkshape/gencode   A.J. Fisher
 * Command line: /www/usr/fisher/helpers/mkfilter -Bu -Bp -o 1 -a 6.2500000000e-02 1.2500000000e-01 -l 
 *
 * Corner frequencies filter 0 = 500Hz 1000Hz
 * Corner frequencies filter 1 = 1500Hz 1750Hz
 * Corner frequencies filter 2 = 2000Hz 2500Hz
 * Corner frequencies filter 3 = 3000Hz 3750Hz
 */
#include "filter.h"

#define NZEROS 2 //Number of zeros in filter
#define NPOLES 2 //Number of poles in filter
#define NUM_OF_FILTERS 4

//Filter Coefficients for filter 0
#define GAIN0 5.962907802e+00
#define A0 -0.6681786379
#define B0 1.4142135624

//Filter Coefficients for filter 1
#define GAIN1 1.115193213e+01
#define A1 -0.8206787908
#define B1 0.5310724140

//Filter Coefficients for filter 2
#define GAIN2 6.026183138e+00
#define A2 -0.6681786379
#define B2 -0.3318213621

//Filter Coefficients for filter 3
#define GAIN3 4.140088960e+00
#define A3 -0.5345111359
#define B3 -1.4142135624

//Filter Coefficients for all filters
float GAIN[NUM_OF_FILTERS] = {GAIN0,GAIN1,GAIN2,GAIN3};
float A[NUM_OF_FILTERS] = {A0,A1,A2,A3};
float B[NUM_OF_FILTERS] = {B0,B1,B2,B3};

//Sample buffers for inputs and outputs
static float xv[NZEROS+1]= {0,0,0}; 
static float yv[NPOLES+1]= {0,0,0};

/*
 * Function:  filter
 * --------------------
 * Filters a signed 8-bit stream, takes one sample and produces one sample
 *
 * filtertype = Butterworth
 * passtype = Bandpass
 * order = 1
 * samplerate = 8000hz
 *
 *  returns: next output sample 
 */
  extern signed char filter(signed char input, int mode)
  { 
    //Shift old input samples
    xv[0] = xv[1]; 
    xv[1] = xv[2]; 

    //Read in new input sample
    xv[2] = (float)input / GAIN1;

    //Shift old input samples
    yv[0] = yv[1]; 
    yv[1] = yv[2]; 

    //Calculate new output
    yv[2] =   (xv[2] - xv[0]) + ( A[mode] * yv[0]) + (  B[mode] * yv[1]);

    //Return new output as a char
    return (signed char)yv[2];
  }
