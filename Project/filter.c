/* Digital filter designed by mkfilter/mkshape/gencode   A.J. Fisher
 * Command line: /www/usr/fisher/helpers/mkfilter -Bu -Bp -o 1 -a 6.2500000000e-02 1.2500000000e-01 -l 
 *
 * filtertype = Butterworth
 * passtype = Bandpass
 * order = 1
 * samplerate = 8000hz
 *
 * Corner frequencies filter 0 = 500Hz 1000Hz
 * Corner frequencies filter 1 = 1500Hz 1750Hz
 * Corner frequencies filter 2 = 2000Hz 2500Hz
 * Corner frequencies filter 3 = 3000Hz 3750Hz
 */
#include "filter.h"
#define NZEROS 2
#define NPOLES 2
#define NUM_OF_FILTERS

#define GAIN0 5.962907802e+00
#define A0 -0.6681786379
#define B0 1.4142135624

#define GAIN1 1.115193213e+01
#define A1 -0.8206787908
#define B1 0.5310724140

#define GAIN2 6.026183138e+00
#define A2 -0.6681786379
#define B2 -0.3318213621

#define GAIN3 4.140088960e+00
#define A3 -0.5345111359
#define B3 -1.4142135624

float GAIN[NUM_OF_FILTERS] = {GAIN0,GAIN1,GAIN2,GAIN3};
float A[NUM_OF_FILTERS] = {A0,A1,A2,A3};
float B[NUM_OF_FILTERS] = {B0,B1,B2,B3};

static float xv[NZEROS+1]; 
static float yv[NPOLES+1];

  extern signed char filter(signed char input, int mode)
  { 
    xv[0] = xv[1]; 
    xv[1] = xv[2]; 
    xv[2] = (float)input / GAIN1;
    yv[0] = yv[1]; 
    yv[1] = yv[2]; 
    yv[2] =   (xv[2] - xv[0]) + ( A[mode] * yv[0]) + (  B[mode] * yv[1]);
    return (signed char)yv[2];
  }
