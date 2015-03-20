/* 
 * ES Coursework Part 2, created by Pontus Petersson, March 2015
 * Embedded Systems, University of Edinburgh
 */
 
#include "fpu.h"
#include "MK70F12.h"

/* 
 * Function:  fpu_init
 * --------------------
 * Enabling access to the Floating-point coprocessor.
 * Coprocessor Access Control Register
 * Fields CP10 and CP11 together control access to the FPU
 * 0b11 = Full access. Note: The effect of writing different values 
 * to CPACR.CP10 and CPACR.CP11 is UNPREDICTABLE.
 */
  void fpu_init()
  {
    SCB_CPACR |= SCB_CPACR_CP10(3) | SCB_CPACR_CP11(3);
  }
