#include "fpu.h"
#include "MK70F12.h"

  void fpu_init()
  {
    SCB_CPACR |= SCB_CPACR_CP10(3) | SCB_CPACR_CP11(3);
  }
