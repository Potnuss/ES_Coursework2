/* periodic interrupt timer delivers interrupts and can be used to poll for TSI */
#include "pit.h"
#include "MK70F12.h"

void pit_init(int time)
{
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; //PIT clock gate control

	PIT_MCR &= ~PIT_MCR_MDIS_MASK; //enable the module clock. This bit must be enabled before any other setup is done.

	
	//Enable PIT interrupt in Nested Vectored Interrupt Controller 
	NVICICPR2 |= 1 << (68 % 32);
	NVICISER2 |= 1 << (68 % 32);
	NVICIP68 |= 3;

	PIT_LDVAL0 = time; //Timer Start Value BitsThese bits set the timer start value

	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enables the timer | Interrupt will be requested whenever TIF is set.

	//No need for Timer Flag Register (PIT_TFLGn)?
}