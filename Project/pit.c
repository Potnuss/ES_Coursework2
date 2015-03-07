/* periodic interrupt timer delivers interrupts and can be used to poll for TSI */
#include "PIT.h"
#include "MK70F12.h"

void pit_init(int time)
{
	// enable PIT interrupt in Nested Vectored Interrupt Controller 
	NVICICPR2 |= 1 << (68 % 32);
	NVICISER2 |= 1 << (68 % 32);
	NVICIP68 |= 3;

	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;

	PIT_MCR = 0;

	PIT_LDVAL0 = time;

	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
	
}