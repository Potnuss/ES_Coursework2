#include "pit.h"
#include "MK70F12.h"

void pit_init(int time)
{
	/*Enable PIT interrupt in Nested Vectored Interrupt Controller 
	PIT: IRQ68 and Vector84*/

	//Clear potentially pending PIT interrupts
	NVICICPR2 |= 1 << (68 % 32); 
	
	//Enable PIT interrupts
	NVICISER2 |= 1 << (68 % 32);

	//Set Priority for PIT interrupts
	NVICIP68 = 0x10; //Group Priority 1



	//PIT clock gate control
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; 

	//Enable the module clock. This bit must be enabled before any other setup is done.
	PIT_MCR &= ~PIT_MCR_MDIS_MASK; 

	//Timer Start Value
	PIT_LDVAL0 = time; 

	//TEN: Enables the timer
	//TIE: Interrupt will be requested whenever TIF is set.
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; 

}
