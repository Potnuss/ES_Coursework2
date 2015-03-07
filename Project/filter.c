/* Digital filter designed by mkfilter/mkshape/gencode   A.J. Fisher
   Command line: /www/usr/fisher/helpers/mkfilter -Bu -Bp -o 1 -a 6.2500000000e-02 1.2500000000e-01 -l */
#include "filter.h"
#define NZEROS 2
#define NPOLES 2
#define GAIN1   5.962907802

static float xv[NZEROS+1]; 
static float yv[NPOLES+1];
/*
extern void filterloop()
  { for (;;)
      { xv[0] = xv[1]; xv[1] = xv[2]; 
        xv[2] = next input value / GAIN;
        yv[0] = yv[1]; yv[1] = yv[2]; 
        yv[2] =   (xv[2] - xv[0])
                     + ( -0.6681786379 * yv[0]) + (  1.4142135624 * yv[1]);
        next output value = yv[2];
      }
  }
*/
  extern signed char filter1(signed char input)
  { 
    xv[0] = xv[1]; 
    xv[1] = xv[2]; 
    xv[2] = (float)input / GAIN1;
    yv[0] = yv[1]; 
    yv[1] = yv[2]; 
    yv[2] =   (xv[2] - xv[0]) + ( -0.6681786379 * yv[0]) + (  1.4142135624 * yv[1]);
    return (signed char)yv[2];
  }
